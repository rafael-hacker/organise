#compdef org

_arguments \
  '(-n --dry-run)'{-n,--dry-run}'[Show what would be done without actually moving files]' \
  '(-w --watch)'{-w,--watch}'[Watch directory for changes and organize continuously]' \
  '(-c --config)'{-c,--config}'[Specify a custom configuration file]:file:_files' \
  '(-u --undo)'{-u,--undo}'[Undo the last organization operation]' \
  '(-h --help)'{-h,--help}'[Show help information]' \
  '(-v --version)'{-v,--version}'[Show version information]' \
  '*:directory:_files -/'
