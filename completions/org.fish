complete -c org -f
complete -c org -s n -l dry-run -d "Show what would be done without moving files"
complete -c org -s w -l watch -d "Watch directory for changes"
complete -c org -s c -l config -d "Specify custom config file" -r
complete -c org -s u -l undo -d "Undo the last operation"
complete -c org -s h -l help -d "Show help"
complete -c org -l version -d "Show version"
complete -c org -s v -l verbose -d "Display detailed execution output"
complete -c org -s r -l recursive -d "Scan directories recursively"
complete -c org -s y -l auto-rename -d "Automatically rename conflicting files, no prompt"
complete -c org -l conflict -d "Set conflict strategy: rename, skip, or overwrite"
