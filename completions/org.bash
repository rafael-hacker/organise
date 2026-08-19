_org_completions() {
    local cur prev opts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"
    opts="-n --dry-run -r --recursive -v --verbose -y --auto-rename --conflict -w --watch -c --config -u --undo -h --help --version"

    if [[ ${cur} == -* ]] ; then
        COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
        return 0
    fi
    
    # Se não for flag, sugere diretórios
    COMPREPLY=( $(compgen -d -- "${cur}") )
}
complete -F _org_completions org
