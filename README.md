# About me
***
I am a linux driver engineer work in a phone maker company, I've been working since 2019-07.

# VIM
***
## 1. Why you need use vim?
 * It is so fast, I can make use of tens of CPUs and much more memory in my company's code server that does not have in my local windows computer.
 * I have used Vscode sourceinsight ever before, all of them costs lots of time to sync my code, but when you use vim in your code server, it only needs serval seconds to generate tags for the whole linux kernel.
 * Flexible customization! you can customize your key mapping as you want.
 * Efficiency tools, like fzf.vim ranger Leaderf rg git related plugins .....
 * save your work time, you can modify your code, build your code, and merge your code at only one terminal!
   here's my vimrc configuration for you, if don't understand some of theme, you can access these AI websites for answer: 
   [deepseek](https://chat.deepseek.com/)    |.   [文心一言](https://yiyan.baidu.com/)    |   [kimi](https://kimi.moonshot.cn/)   |.  [蓝心千寻](https://qianxun.vivo.com/#/explore) | [字节豆包](https://www.doubao.com/chat/)   |   [copilot](https://github.com/copilot)   |.  [讯飞星火](https://xinghuo.xfyun.cn/desk)
   
```c
set runtimepath^=~/.vim
call plug#begin('~/.vim/plugged')
Plug 'rbgrouleff/bclose.vim'
Plug 'skywind3000/asyncrun.vim'
Plug 'Yggdroot/LeaderF'

Plug 'vim-scripts/taglist.vim'
Plug 'tomasr/molokai'
Plug 'preservim/nerdtree'

Plug 'junegunn/fzf', { 'do': { -> fzf#install() } }
Plug 'junegunn/fzf.vim'

Plug 'easymotion/vim-easymotion'
Plug 'haya14busa/incsearch-easymotion.vim'

Plug 'git@github.com:vim-scripts/c.vim.git'
Plug 'vim-scripts/c.vim'

Plug 'voldikss/vim-floaterm'
Plug 'francoiscabrol/ranger.vim'

Plug 'matze/vim-move'

Plug 'tpope/vim-fugitive'
Plug 'airblade/vim-gitgutter'
Plug 'junegunn/gv.vim'

Plug 'terryma/vim-multiple-cursors'
Plug 'mg979/vim-visual-multi'

Plug 'preservim/tagbar'
Plug 'kshenoy/vim-signature'
Plug 'terryma/vim-expand-region'
call plug#end()
filetype indent off

syntax on
syntax enable
colorscheme molokai

set notermguicolors
set t_Co=256
set mouse=a
set hlsearch
set ai
set cindent
set linebreak
set nocompatible
set sidescroll=5
set encoding=utf-8
set showcmd
set ruler
set cursorline
set showtabline=0
if has('nvim')
set cursorlineopt=number
else
endif
set incsearch
set showmatch
set matchtime=10
set nowrapscan
set helplang=cn
set langmenu=zh_CN.UTF-8
set noswapfile
set nowb
set mousemodel=extend
set smartcase
set signcolumn=yes
set title
set tabstop=4
set shiftwidth=4
set nowritebackup
set nobackup
set noautowrite
set history=50
set copyindent
set ttyfast
set report=0
set synmaxcol=200
set autoread
set foldcolumn=0
set lazyredraw
set lbr
set tw=500
set ai "Auto indent
set nu
set relativenumber
set si "Smart indent
set nowrap "Wrap lines
set switchbuf+=useopen
set switchbuf+=usetab
set wildignore+=.*
set foldenable
set foldmethod=manual
set sessionoptions-=options
set completeopt=longest,menu
set list
set listchars=tab:›\ ,trail:.,extends:#,nbsp:.
set dictionary+=~/.vim/words
set fileencodings=utf-8,gbk,gb18030,gb2312,ucs-bom,cp936,latin1
set hid
set pastetoggle=<F10>
set laststatus=2
set statusline+=%#StatusLinePath#%{getcwd()}/%m/%{fugitive#statusline()}/%{&fenc}/%b/%#StatusLineFileName#%t

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
iab xdate <C-r>=strftime("20%y-%m-%d %H:%M:%S")<cr>

function! DeleteVisualMatch()
    " 获取可视模式下选中的内容
    let [line_start, col_start] = getpos("'<")[1:2]
    let [line_end, col_end] = getpos("'>")[1:2]
    let lines = getline(line_start, line_end)
    if len(lines) == 0
        return
    endif
    let lines[-1] = lines[-1][: col_end - 1]
    let lines[0] = lines[0][col_start - 1:]
    let selected_text = join(lines, "\n")

    " 查找匹配 ID 并删除
    for match in getmatches()
        if match['pattern'] == selected_text
            call matchdelete(match['id'])
            echo "Deleted match ID: " . match['id']
            return
        endif
    endfor
    echo "No match found for selected text."
endfunction

function! HighlightWithColorCode(color_code) range
	let start_line = a:firstline
	let end_line = a:lastline
	let start_col = col("'<")
	let end_col = col("'>")
	let selected_text = getline(start_line)[start_col-1 : end_col-1]

	if  a:color_code == 999
		echo "clear all"
	call clearmatches()
		return
	endif

	if a:color_code < 0 || a:color_code > 255
		echo "Invalid color code: " . a:color_code
		return
	endif

	let hl_group = 'MyColor' . a:color_code
	try
		execute 'highlight ' . hl_group . ' ctermbg=' . a:color_code . ' guibg=' . a:color_code
	catch /E254/
		echo "color:" . a:color_code
	endtry

	call matchadd(hl_group, selected_text)
	endfunction

command! -range -nargs=1 HighlightWithColorCode call HighlightWithColorCode(<f-args>)

function! LoadTags()
	call system("mv tags/* .")
	let tags = ""
	for tagfile in split(glob("tag_2025_Will_Rich_*"), "\n")
		if stridx(&tags, tagfile) == -1
			let tags .= tagfile . ","
		endif
	endfor

	let tags = substitute(tags, ',$', '', '')
	execute "set tags+=" . tags
endfunction

function! GenerateTags()
	call system("mkdir tags")
	execute 'silent AsyncRun -mode=0 bash ~/.vim/tags.sh'
	autocmd User AsyncRunStop silent! call LoadTags()
endfunction

function! Togglefunc1()
  execute g:param1 ? "set nu relativenumber list" : "set nonu norelativenumber nolist"
  let g:param1 = !g:param1
endfunction

function! s:config_easyfuzzymotion(...) abort
  return extend(copy({
  \   'converters': [incsearch#config#fuzzyword#converter()],
  \   'modules': [incsearch#config#easymotion#module({'overwin': 1})],
  \   'keymap': {"\<CR>": '<Over>(easymotion)'},
  \   'is_expr': 0,
  \   'is_stay': 1
  \ }), get(a:, 1, {}))
endfunction

function! s:execute_selected_command(command)
  execute a:command
endfunction

function! s:execute_command()
 let g:command_list = [
  \ ':Git blame',
  \ ':BLines',
  \ ':LeaderfLine',
  \ ':Lines',
  \ ':BCommits',
  \ ':Commits',
  \ ':Rg',
  \ ]
 call fzf#run(fzf#wrap({
  \ 'source': g:command_list,
  \ 'sink': function('s:execute_selected_command')
  \ }))
endfunction

command! -bang -nargs=* Rg
  \ call fzf#vim#grep(
  \   'rg --threads 79 -L  --column --line-number --no-heading --color=always --smart-case  '.shellescape(<q-args>), 1,
  \   fzf#vim#with_preview(), <bang>0)

let g:fzf_preview_window = ['up:50%', 'ctrl-/']
let g:fzf_layout = { 'window': 'enew' }
let g:fzf_vim = {}
let g:fzf_follow_links = 1
let g:fzf_vim_dir = expand('%:p:h')
let $FZF_DEFAULT_OPTS='--height 100% --layout=reverse'
let g:fzf_buffers_jump = 1
let g:fzf_action = {
  \ 'ctrl-x': 'split',
  \ 'ctrl-v': 'vsplit',
  \ 'ctrl-t': 'tabnew' }

let mapleader = " "
let g:Lf_FollowLinks = 1
let NERDTreeOpenOnStartup=0
let NERDTreeWinPos="left"
let g:param1= 0
let g:initial_directory_set = 0

let g:tagbar_position = 'left'
let Tlist_WinWidth = 50
let Tlist_Use_SingleClick = 1
let Tlist_Use_Right_Window = 0
let Tlist_WinWidth = 50

let g:tagbar_singleclick = 1
let g:tagbar_autoclose = 0

let g:EasyMotion_do_mapping = 0
let g:EasyMotion_smartcase = 1
let g:EasyMotion_use_smartsign_us = 1 " US layout
let $FZF_DEFAULT_OPTS='--height 100% --layout=reverse'
let g:move_key_modifier_visualmode = 'S'
let loaded_matchparen=1
let g:ranger_map_keys = 0

let g:floaterm_width = 0.99  " 宽度为屏幕的 80%
let g:floaterm_height = 0.99 " 高度为屏幕的 60%
let g:floaterm_autoinsert = 1
let g:floaterm_autohide = 1
let g:floaterm_autoclose = 2
let current_file_dir = expand('%:p:h')
let current_working_dir = getcwd()
let g:ranger_replace_netrw = 1

cabbrev rg Rg
cabbre git Git

nnoremap <leader><leader> :LeaderfBufTag<CR>
nnoremap <leader>l :LeaderfFunction<CR>
nnoremap <leader>L :BTags<CR>

nnoremap <leader>f :Files<CR>
nnoremap <leader>F :LeaderfFile<CR>

nnoremap <leader>R :Buffers<CR>
nnoremap <leader>r :LeaderfBuffer<CR>

nnoremap <C-h> :tabprevious<CR>
nnoremap <C-l> :tabnext<CR>
vnoremap <C-h> :tabprevious<CR>
vnoremap <C-l> :tabnext<CR>

nnoremap <C-@> :
nnoremap <C-w> <C-w>w
nnoremap u :u<cr>:w<cr>
vnoremap < <gv
vnoremap > >gv
vnoremap $ $h

vnoremap y "ay
vnoremap p "ap
nnoremap p "ap
nnoremap Z ZZ
inoremap jj <Esc>
vnoremap <Rightmouse> "ay
nnoremap <Rightmouse> "aP

vnoremap <Up> <Nop>
vnoremap <Down> <Nop>
vnoremap <Left> <Nop>
vnoremap <Right> <Nop>

nnoremap <Up> <Nop>
nnoremap <Down> <Nop>
nnoremap <Left> <Nop>
nnoremap <Right> <Nop>

inoremap <Up> <Nop>
inoremap <Down> <Nop>
inoremap <Left> <Nop>
inoremap <Right> <Nop>

inoremap <C-h> <left>
inoremap <C-j> <down>
inoremap <C-k> <up>
inoremap <C-l> <right>

nnoremap <C-Space> :
inoremap <C-Space> <C-n>

nnoremap <S-h> :vertical resize -3<CR>
nnoremap <S-l> :vertical resize +3<CR>
nnoremap <S-j> :resize +3<CR>
nnoremap <S-k> :resize -3<CR>
nnoremap <silent> <leader>no :call Togglefunc1()<CR>
vnoremap<C-c> :w! ~/.vim/cvbuf.c<CR>
nnoremap<C-v> :r ~/.vim/cvbuf.c<CR>

nnoremap a <C-]>zz
nnoremap # #N
nnoremap n nzz
nnoremap N Nzz
nnoremap j jzz
nnoremap k kzz
nnoremap w wzz

nnoremap <silent><expr> <Space>/ incsearch#go(<SID>config_easyfuzzymotion())
nnoremap<leader>t :FloatermToggle<CR>
vnoremap <S-j> :m '>+1<CR>gv=gv
vnoremap <S-k> :m '<-2<CR>gv=gv
nnoremap f <C-o>zz
nnoremap a <C-]>zz
nnoremap F [[k2wzz

nnoremap <leader>s :Rg <C-r><C-w> = <CR>
vnoremap <leader>s y:Rg <C-r>" = <CR>
vnoremap R y:Rg <C-r>"<CR>
nnoremap R :Rg <C-r><C-w><CR>

vnoremap h :<C-u>call HighlightWithColorCode(input('Enter color code (0-255): '))<CR>
vnoremap <silent> # y/<C-r>"<CR>Nzz
nnoremap <F1> 5zh
nnoremap <F2> 5zl
vnoremap <F1> 5zh
vnoremap <F2> 5zl
inoremap <F1> 5zh
inoremap <F2> 5zl
noremap j gj
noremap k gk
vnoremap <Leader>dm :call DeleteVisualMatch()<CR>


map ce :Rg<cr>
map cf :NERDTreeFind<cr>
map cc :TagbarToggle<cr>
map cs :sp<cr>
map cv :vsp<cr>
map ca :Ranger<cr>
map cq :copen 20<cr>
map cL :call system('rm -rf *2025_Will_Rich_* tags \n')<cr>
map cl :colorscheme default<bar>syntax off<cr>
map co :colorscheme molokai<bar>syntax on<cr>
map ct :call GenerateTags()<cr>
map E <Plug>(expand_region_expand)
map t :call <SID>execute_command()<CR>

autocmd BufWritePost * GitGutter
autocmd InsertLeave * silent! write
autocmd FocusGained,BufEnter * silent! checktime
autocmd BufRead,BufNewFile * source ~/.vim/plugged/c.vim/ftplugin/c.vim
autocmd BufRead,BufNewFile * source ~/.vim/plugged/c.vim/ftplugin/make.vim
autocmd BufRead,BufNewFile *  hi LineNr ctermfg=yellow
autocmd BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g'\"" | endif
autocmd InsertLeave * if pumvisible() == 0|pclose|endif
autocmd vimEnter * execute 'cd' expand('%:p:h')
autocmd VimLeave * call system("mv *_2025_Will_Rich_* tags/")
autocmd BufRead * silent! call LoadTags()
autocmd Syntax * source ~/.vim/plugged/c.vim/ftplugin/c.vim
autocmd User FloatermExit :FloatermNew --autoclose=0 exit 0
autocmd BufRead,BufNewFile *  hi clear SignColumn
autocmd BufRead,BufNewFile *  hi clear LineNr
autocmd BufEnter * if !g:initial_directory_set | silent! lcd %:p:h | let g:initial_directory_set = 1 | endif

hi TabLine      ctermfg=2 ctermbg=233
hi TabLineSel   ctermfg=118
hi TabLineFill  ctermfg=233
hi ModeMsg ctermbg=233 ctermfg=2
hi MoreMsg ctermbg=233 ctermfg=2
hi Question ctermbg=233 ctermfg=2
hi StatusLineFileName ctermfg=119
hi CursorLineNr ctermfg=2
hi StatusLinePath ctermbg=233 ctermfg=2
hi Search ctermfg=white ctermbg=darkblue
let g:python3_host_prog = '~/venv/bin/python3'
```

Heres the code looks like:
![image](https://github.com/user-attachments/assets/f7899267-2516-470f-8d39-c12569bb0eb7)


# tmux
***
## Why you need tmux?
* It can keep your work safety even if your computer is power off
* It can create multi sessions/windows/panes as you want and do different work in each place.
* You can change your session/windows/panes as you want
* most importantly, you can login your ssh account only once

  Here's the basic tmux config for you:
  
![image](https://github.com/user-attachments/assets/4ef6e2d9-31c1-47f0-ae3e-5ad2ee82e9e2)

Real image
![image](https://github.com/user-attachments/assets/fbf89bf2-a92d-4683-9efd-94795f0eaf52)




