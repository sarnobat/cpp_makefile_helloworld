#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "globe_jpg.h" // generated via `xxd -i globe.jpg`

struct Point { float x, y, z; };
struct Bounds { float minX, maxX, minY, maxY, minZ, maxZ; };

static std::vector<Point> points;

static Bounds compute_bounds() {
    Bounds b{};
    if (points.empty()) return b;
    b.minX=b.maxX=points[0].x; b.minY=b.maxY=points[0].y; b.minZ=b.maxZ=points[0].z;
    for (const auto &p : points) {
        if (p.x<b.minX) b.minX=p.x; if (p.x>b.maxX) b.maxX=p.x;
        if (p.y<b.minY) b.minY=p.y; if (p.y>b.maxY) b.maxY=p.y;
        if (p.z<b.minZ) b.minZ=p.z; if (p.z>b.maxZ) b.maxZ=p.z;
    }
    return b;
}

static void load_csv(const char *path) {
    FILE *f=fopen(path,"r");
    if(!f){ perror("open csv"); exit(1);}
    char line[256];
    while(fgets(line,sizeof(line),f)){
        float x,y,z; if(sscanf(line,"%f,%f,%f",&x,&y,&z)!=3) continue;
        points.push_back({x,y,z});
    }
    fclose(f);
    fprintf(stderr,"Loaded %zu points from %s\n",points.size(),path);
}

static void error_callback(int error,const char *desc){ fprintf(stderr,"GLFW error: %s\n",desc); }

static void draw_bounding_box(const Bounds &b) {
    glColor3f(1.0f,0.0f,0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex3f(b.minX,b.minY,b.minZ); glVertex3f(b.maxX,b.minY,b.minZ);
    glVertex3f(b.maxX,b.minY,b.minZ); glVertex3f(b.maxX,b.minY,b.maxZ);
    glVertex3f(b.maxX,b.minY,b.maxZ); glVertex3f(b.minX,b.minY,b.maxZ);
    glVertex3f(b.minX,b.minY,b.maxZ); glVertex3f(b.minX,b.minY,b.minZ);
    glVertex3f(b.minX,b.maxY,b.minZ); glVertex3f(b.maxX,b.maxY,b.minZ);
    glVertex3f(b.maxX,b.maxY,b.minZ); glVertex3f(b.maxX,b.maxY,b.maxZ);
    glVertex3f(b.maxX,b.maxY,b.maxZ); glVertex3f(b.minX,b.maxY,b.maxZ);
    glVertex3f(b.minX,b.maxY,b.maxZ); glVertex3f(b.minX,b.maxY,b.minZ);
    glVertex3f(b.minX,b.minY,b.minZ); glVertex3f(b.minX,b.maxY,b.minZ);
    glVertex3f(b.maxX,b.minY,b.minZ); glVertex3f(b.maxX,b.maxY,b.minZ);
    glVertex3f(b.maxX,b.minY,b.maxZ); glVertex3f(b.maxX,b.maxY,b.maxZ);
    glVertex3f(b.minX,b.minY,b.maxZ); glVertex3f(b.minX,b.maxY,b.maxZ);
    glEnd();
}

static void perspective(float fovY,float aspect,float near,float far,float out[16]){
    float f=1.0f/tanf(fovY*0.5f*M_PI/180.0f);
    out[0]=f/aspect; out[1]=out[2]=out[3]=0;
    out[4]=0; out[5]=f; out[6]=out[7]=0;
    out[8]=out[9]=0; out[10]=(far+near)/(near-far); out[11]=-1;
    out[12]=out[13]=0; out[14]=2*far*near/(near-far); out[15]=0;
}

static void lookAt(float ex,float ey,float ez,float cx,float cy,float cz,float ux,float uy,float uz,float out[16]){
    float fx=cx-ex,fy=cy-ey,fz=cz-ez; float r=1.0f/sqrtf(fx*fx+fy*fy+fz*fz); fx*=r; fy*=r; fz*=r;
    float sx=fy*uz-fz*uy,sy=fz*ux-fx*uz,sz=fx*uy-fy*ux; r=1.0f/sqrtf(sx*sx+sy*sy+sz*sz); sx*=r; sy*=r; sz*=r;
    float vx=sy*fz-sz*fy,vy=sz*fx-sx*fz,vz=sx*fy-sy*fx;
    out[0]=sx; out[1]=vx; out[2]=-fx; out[3]=0;
    out[4]=sy; out[5]=vy; out[6]=-fy; out[7]=0;
    out[8]=sz; out[9]=vz; out[10]=-fz; out[11]=0;
    out[12]=-(sx*ex+sy*ey+sz*ez);
    out[13]=-(vx*ex+vy*ey+vz*ez);
    out[14]=fx*ex+fy*ey+fz*ez;
    out[15]=1;
}

// ===== Map Support =====
static GLuint mapTex = 0;

static void load_map_texture() {
    int w,h,n;
    unsigned char *img = stbi_load_from_memory(globe_jpg, globe_jpg_len, &w,&h,&n,3);
    if(!img){ fprintf(stderr,"Failed to load embedded map\n"); return; }
    glGenTextures(1,&mapTex);
    glBindTexture(GL_TEXTURE_2D,mapTex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,w,h,0,GL_RGB,GL_UNSIGNED_BYTE,img);
    stbi_image_free(img);
}

static void draw_map(const Bounds &b) {
    if(!mapTex) return;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,mapTex);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
    float sizeX = b.maxX - b.minX;
    float sizeZ = b.maxZ - b.minZ;
    float cx = (b.maxX + b.minX)/2.0f;
    float cz = (b.maxZ + b.minZ)/2.0f;
    float y = b.minY - (sizeX+sizeZ)*0.05f;
    glTexCoord2f(0,0); glVertex3f(cx - sizeX, y, cz - sizeZ);
    glTexCoord2f(1,0); glVertex3f(cx + sizeX, y, cz - sizeZ);
    glTexCoord2f(1,1); glVertex3f(cx + sizeX, y, cz + sizeZ);
    glTexCoord2f(0,1); glVertex3f(cx - sizeX, y, cz + sizeZ);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

// ===== Main =====
int main(int argc,char**argv){
    if(argc<2){ fprintf(stderr,"Usage: %s data.csv [--map]\n",argv[0]); return 1;}
    bool show_map=false;
    for(int i=1;i<argc;i++) if(strcmp(argv[i],"--map")==0) show_map=true; else load_csv(argv[i]);

    Bounds b=compute_bounds();
    float cx=(b.minX+b.maxX)/2, cy=(b.minY+b.maxY)/2, cz=(b.minZ+b.maxZ)/2;
    float maxE=fmax(b.maxX-b.minX,fmax(b.maxY-b.minY,b.maxZ-b.minZ));
    float camX=cx+maxE*1.5f, camY=cy+maxE*1.5f, camZ=cz+maxE*1.5f;

    glfwSetErrorCallback(error_callback);
    if(!glfwInit()) return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
    GLFWwindow*win=glfwCreateWindow(800,600,"Particles",NULL,NULL);
    if(!win){glfwTerminate();return 1;}
    glfwMakeContextCurrent(win);
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glPointSize(5.0f);
    glClearColor(0.05f,0.05f,0.1f,1.0f);

    if(show_map) load_map_texture();

    float angle=0;
    while(!glfwWindowShouldClose(win)){
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        float proj[16]; perspective(60.0f,800.0f/600.0f,0.1f,1000.0f,proj);
        glMatrixMode(GL_PROJECTION); glLoadMatrixf(proj);
        float view[16]; lookAt(camX,camY,camZ,cx,cy,cz,0,1,0,view);
        glMatrixMode(GL_MODELVIEW); glLoadMatrixf(view);

        glPushMatrix();
        glRotatef(angle,0,1,0);
        if(show_map) draw_map(b);
        draw_bounding_box(b);
        glBegin(GL_POINTS);
        for(const auto&p:points){ glColor3f(0.7f,0.8f,1.0f); glVertex3f(p.x,p.y,p.z);}
        glEnd();
        glPopMatrix();
        glfwSwapBuffers(win);
        glfwPollEvents();
        angle+=0.3f;
    }
    glfwTerminate();
    return 0;
}
