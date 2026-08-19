#compdef org

_arguments \
  '(-n --dry-run)'{-n,--dry-run}'[Show what would be done without actually moving files]' \
  '(-w --watch)'{-w,--watch}'[Watch directory for changes and organize continuously]' \
  '(-c --config)'{-c,--config}'[Specify a custom configuration file]:file:_files' \
  '(-u --undo)'{-u,--undo}'[Undo the last organization operation]' \
  '(-v --verbose)'{-v,--verbose}'[Display detailed execution output]' \
  '(-y --auto-rename)'{-y,--auto-rename}'[Automatically rename conflicting files, no prompt] \
  '(-r, --recursive)'{-r, --recursive}'[Scan directories recursively] \
  '(-h --help)'{-h,--help}'[Show help information]' \
  '(--version)'{--version}'[Show version information]' \
  '*:directory:_files -/'
