 #include "common.h"
#include <iostream>

int main() {
    mq_unlink(MQ_CLIENTS);
    mq_unlink(MQ_MID);
    mq_unlink(MQ_OUT);
    std::cout << "Unlinked queues (if they existed).
";
    return 0;
}
