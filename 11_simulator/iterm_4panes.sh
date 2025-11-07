cat <<EOF > /tmp/4panes.applescript
tell application "iTerm"
    -- create a new window
    create window with default profile
    tell current window
        set myTab to current tab

        -- pane 1 (top-left)
        set pane1 to current session of myTab
        tell pane1
            write text "cd /Volumes/git/github/depot_parent/cpp/11_simulator && make run-server-ingest"
        end tell

        -- split vertically -> right column (top-right)
        set pane2 to split vertically with default profile of pane1
        tell pane2
            write text "cd /Volumes/git/github/depot_parent/cpp/11_simulator && make run-server-forward"
        end tell

        -- split pane1 horizontally -> bottom-left
        set pane3 to split horizontally with default profile of pane1
        tell pane3
            write text "cd /Volumes/git/github/depot_parent/cpp/11_simulator && make run-receiver"
        end tell

        -- split pane2 horizontally -> bottom-right
        set pane4 to split horizontally with default profile of pane2
        tell pane4
            write text "cd /Volumes/git/github/depot_parent/cpp/11_simulator && make run-clients"
        end tell
    end tell
end tell

EOF

osascript /tmp/4panes.applescript