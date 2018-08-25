# Some tools for shared server during contests

## Auto chmod

During contests, we use SSH server to share files through `/home/share/` folder (with sticky bit; it is out of scope of this document at the moment). Each user has to chmod their files to make them readable for other users. It is not convenient when the work is intensive. So we chmod the files automatically.

Regular approach with `while :; do chmod -R a+r /home/share/* ; done` is not safe due to symlinks. Moving files out of the shared folder could be a solution, but it may confuse users. So a special tool was developed.

- `chmod_nofollow.c` is the tool to chmod files that does not follow symlinks.
- `0_root_chmod.sh` is a wrapper to call the tool frequently.

## Report bugs

Bug reports are welcome!
