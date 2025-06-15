# Astro 331 instructor setup

## required software

You will need this software to run Astro 331. Most software can be installed from the command line with Windows’ built-in `winget` command. Those marked with an asterisk require a separate download and installation. 

- XCTU* (https://hub.digi.com/support/products/xctu/)
  configure the radios used for the comm lab
- Markdown software
  - my favorite is obsidian--the free version is sufficient
  - my other favorite is typora* (https://typora.io/), but it’s not free
- version control
  - git (`winget install git.git`) version control and distribution of course material to lab/student computers
    - interfaces with github.com (https://github.com/usafa-astro-331/Astro-331X-2023)

  - Tortoisegit (`winget install Tortoisegit`) graphical interface for git—integrates with Windows’ File Explorer
  - You may also want a robust git GUI. My favorite is Sourcetree* (https://sourcetreeapp.com/?v=win). It’s free, but requires an account from atlassian/bitbucket to activate. 

- Arduino IDE (`winget install ArduinoSA.IDE.stable`)
- bash (linux command line)—execute some shell scripts
  - to get a bash terminal on windows, install ubuntu from the windows store. You can do this from the windows command line: `winget install 9PDXGNCFSCZV`


## documentation

Documentation is written in markdown. Markdown is a simple markup language that still displays nicely in a text editor if necessary. 

Instructions for each lab are found in that folder’s `README.md`. Github automatically display’s the contents of that file when you view the folder online. 

Images can be added to the markdown file using relative paths, and then they will display locally and online at github. 

## file types

Text-based files work better in version control repositories than binary files. 

For files that will never change (datasheets), there is no problem storing binary files such as pdfs. For files that will change regularly (lab instructions), use an uncompressed text-based format. 

|                                           | good  | medium | bad    |
| ----------------------------------------- | ----- | ------ | ------ |
| documentation                             | *.md  | *.rtf  | *.docx |
| matlab code                               | *.m   | *.mlx  |        |
| data, lists                               | *.csv |        | *.xlsx |
| vector images                             | *.svg |        | *.pdf  |
| single-version docs <br />e.g. datasheets | *.pdf |        |        |
| fritzing diagrams                         | *.fz  |        | *.fzz  |



## XBee radio setup

The communication lesson (and maybe additional lessons in the future!) uses XBee radios which use the 802.15.4 protocol and operate in the 2.4 GHz ISM band. Each group has two radios, and each radio is attached to an XBee explorer adapter, which provides breadboard-spaced pins and a USB connection. 

One XBee is mounted to FlatSAT, and the other connects to a PC via a USB cable. The PC sees the XBee as a USB serial port, and connects via any serial terminal. Currently, the lab instructions call for PuTTY Portable.

Each pair of radios needs to be configured to communicate with its partner. The group-specific changes (PAN ID/channel number) are made by students following the lab instructions. However, each pair of radios needs the same firmware and base configuration before the lab. 

- install XCTU from digi: https://hub.digi.com/support/products/xctu/
- connect 1 (or many) XBees to your computer via USB
- **Discover** radio modules (big icon with a magnifying glass)
  - select all, next, next, etc. 
- select an XBee from the list on the left
- **Update** the firmware
  - function set: Digi Xbee3 802.15.4
  - 200D (current as of 2023 summer)
- **Default**: restore default firmware settings
- **Write** radio settings



### settings

Astro 331 uses transparent mode (AP0). To both ends this looks like any other serial connection, and characters are passed as they are typed (including backspace, which is transmitted as the backspace character).

All settings on both radios should be identical except the following. 

|      | A (ground) | B (FlatSAT) |
| ---- | ---------- | ----------- |
| DH   | 0          | 0           |
| DL   | 1          | 0           |
| MY   | 0          | 1           |

Communication requires that both radios use the same *channel* and *network PAN ID*.  

#### channel

Both ends should be on the same channel. The default channel of 0x0C (12 in decimal) is acceptable. Theoretically different groups will interfere with each other on this channel and reduce available bandwidth, but

1. throughput is so low that the chance of interference is basically zero
2. there’s so much other interference on 2.4 GHz (wifi, microwaves) that it doesn’t matter. 

#### PAN ID (network ID)

Each group will choose a PAN ID from 0x0 to 0xFFFF. Each group’s PAN ID must be unique. 

For simplicity, the lab instructions direct the students to select a decimal number from 0 to 9999. 


## git, Tortoisegit, github

You will use git for version control and distribution of course material. Git is a command line program, so you may prefer to use Tortoisegit, which provides integration with Windows’ File Explorer. Finally, you will use github as the remote repository to distribute course material. 

Interacting with git/github will look just like working with files and folders o n you PC, but with a few added steps. 

Each instance of git material is called a repository. On github, a repository looks like a website. On your PC, a repository looks like any other folder. 

I use github as the master remote repository. Everybody will sync to that one.

### setup

- If creating a new repository:
  - Create a new repository on github.com. For this course, I’ve been creating them under the organization https://github.com/usafa-astro-331
- copy the repository’s URL: https://github.com/usafa-astro-331/Astro-331X-2023
- go to the location on your PC where you want to place the repository (i.e. folder), for example, My Documents
- right click -> git clone -> paste URL -> OK

### use

When you start work for the day, you should pull from github in case someone else made changes. 

- right click -> git sync -> pull



Here’s the overview of your workflow as you work with a git repository. 

- **work**
- **save** file
- **add** new or changed files to git’s staging area
  - in Tortoisegit this is just checking the box next to a file in the commit dialog
- **commit** added files to your local repository
- **push** your changes to the remote repository (github) 

It looks complicated, but 

1) it’s less complicated than it seems
2) it will be completely natural in 2-3 days
3) it *vastly* simplifies course material distribution



#### work & save

As you work, create and save files just as you always have. Don’t create new names for new versions of a file—git stores all of the past versions that were committed, so just overwrite old files as necessary.  

A file (or new version of a file) doesn’t become a version of record until you commit it to the git repository. Ensure that your code works/compiles before you commit it. 

#### add & commit

When you’re done working on a file (or files), or when you just want to record the current state of the project, commit your changes to the local repository. 

Git will require a commit message for each commit. This can be as simple as a single typed space, but you usually want to provide a sentence describing the changes you’ve made. 

Git creates a unique label for each commit—a number that uniquely identifies that commit within a repository. The last one for this repository was `c9199e9`. You can use this as a version number if you need to point someone to a specific version of your project. 

#### push

Push your changes to the remote repository. 

Occasionally, your push will fail. Usually this is because somebody else made changes that your local repository doesn’t yet know about. To fix this, pull changes from the remote repository, and then try again.

Currently (2023) course repositories are public. The public (and students) only have read access. 

With the number of people who might be making changes (1–2 instructors), the chance of conflicts is small. Even if we’re working on the same file, we’re probably not changing the same parts, and git is smart enough to merge most changes automatically. 

Also, git’s error messages are usually explicit and helpful. If you don’t immediately recognize the problem, the answer is usually a quick search away. 

## Arduino IDE

It’s an IDE. You’ll figure out how to use it. However, this particular IDE has a few limitations that caught me by surprise. 

- The file name of a sketch (`*.ino`) must exactly match the name of the folder it’s stored in. This means you can’t store two sketches in the same folder. 
- The file name (and therefore also the folder name) can’t contain spaces. Older versions of Arduino (before 2.0) *did* allow spaces, but then all of those sketches stop compiling when you upgrade to 2.0. 
- You can’t `#include` files from outside the current folder. Exception: you *can* include libraries installed in your computer’s Arduino library folder. 

## component list

Each lab has a parts list saved as a csv file. The list for each lab assumes that you already have the parts for the previous lab—a lab may include components that aren’t explicitly listed in that lab’s `components.csv`.

To create or update the consolidated part list for the course, run `create_part_list.sh`. The output is `components.csv` in the repository root (the main folder). 
