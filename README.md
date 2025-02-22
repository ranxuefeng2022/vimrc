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
   here's my vimrc configuration for you: [vimrc](https://github.com/ranxuefeng2022/vimrc/blob/main/config/vimrc)
   If don't understand some of theme, you can access these AI websites for answer: 
   [deepseek](https://chat.deepseek.com/)    |.   [文心一言](https://yiyan.baidu.com/)    |   [kimi](https://kimi.moonshot.cn/)   |.  [蓝心千寻](https://qianxun.vivo.com/#/explore) | [字节豆包](https://www.doubao.com/chat/)   |   [copilot](https://github.com/copilot)   |.  [讯飞星火](https://xinghuo.xfyun.cn/desk) or click 
   


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
```
set-option -g mouse on
set-option -g status on
set-option -g history-limit 10000
bind-key -n M-x resize-pane -Z
bind-key -n M-a run-shell "tmux select-pane -t :.+ && tmux resize-pane -Z"
bind-key -n M-w select-window -t :-
bind-key -n M-s switch-client -n
unbind C-a
set -g prefix C-a
bind C-a send-prefix
```

## tmux Breif workflow
![image](https://github.com/user-attachments/assets/4ef6e2d9-31c1-47f0-ae3e-5ad2ee82e9e2)


***
# If you think this is useful, you can buy me a coffe ^-^
![image](https://github.com/user-attachments/assets/cc01d7ac-aad6-4e59-8da4-a6b4a5c187ce)


