
" Key bindings for vim-lsp.
nn <silent> <M-d> :LspDefinition<cr>
nn <silent> <M-r> :LspReferences<cr>
nn <f2> :LspRename<cr>
nn <silent> <M-a> :LspWorkspaceSymbol<cr>
nn <silent> <M-l> :LspDocumentSymbol<cr>

" Vim-Plug plugins
call plug#begin('~/.vim/plugged')

" Colour scheme
"Plug 'catppuccin/vim', { 'as': 'catppuccin' }
"Plug 'morhetz/gruvbox'
Plug 'ghifarit53/tokyonight-vim'

Plug 'sheerun/vim-polyglot'

" Vertical lines for indentations
Plug 'https://github.com/Yggdroot/indentLine'

" Vim language server
Plug 'https://github.com/prabirshrestha/vim-lsp'

" Auto-install language servers
Plug 'https://github.com/mattn/vim-lsp-settings'

" Semantic highlighting for lsp
Plug 'https://github.com/jackguo380/vim-lsp-cxx-highlight'

" Auto complete for LSP
Plug 'https://github.com/prabirshrestha/asyncomplete-lsp.vim'

" Arduino plugin
Plug 'https://github.com/stevearc/vim-arduino'

call plug#end()

" For polygot
set nocompatible

" Use spaces to indent instead of tabs
:set tabstop=4
:set shiftwidth=4
:set expandtab

" Show line-numbering
:set number

" Vertical line-length ruler
:set colorcolumn=80
:highlight clear
:highlight ColorColumn guibg=black

"For Colourscheme
"set background=dark
"set termguicolors 

"colorscheme catppuccin_mocha
"autocmd vimenter * ++nested colorscheme gruvbox

" Colourscheme gruvbox
"let g:gruvbox_italic=1
"let g:gruvbox_termcolors=16
"colorscheme gruvbox

" Colourscheme tokyonight
set termguicolors
let g:tokyonight_style = 'night' " available: night, storm
let g:tokyonight_enable_italic = 1
colorscheme tokyonight

" Mapping keys to comment and uncomment code (in C/C++/javascript).
map <c-_> :s/^/\/\/<CR>
map <c-\> :s/^\/\//<CR>

" Mapping keys for indent.
map <Tab> :s/^/<Tab><CR>
map <S-Tab> :s/^<Tab>/<CR>

let g:arduino_serial_cmd = 'screen {port} {baud}'
let g:arduino_serial_cmd = 'picocom {port} -b {baud} -l'
