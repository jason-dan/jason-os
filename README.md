# jason-os
A prototype operating system written in the C programming language! Contains kernel and shell features for file system I/O, volume partitioning and mounting, shell memory I/O, and concurrent process execution. Had a blast making this for my operating systems course at McGill.

## Commands and Features
### Shell Memory I/O
We've got shell memory! Assign or reassign a string to a shell memory variable using `set VAR STRING`. Print the contents of the variable to the console with `print VAR`

### Run scripts!
Got a script on a text file that you wanna run? No problem. jason-os has you covered baby! Execute entire scripts with `run SCRIPT` (script being the path to the script file).

It gets better. Run up to three scripts concurrently! Just put `exec SCRIPT1 SCRIPT2 SCRIPT3` (replace with the actual script files), and jason-os will run all three! See next point for more.

### CPU and RAM emulation, scheduling, paging, concurrent execution
We've got CPU emulation. We've got schedulers and ready queues for that CPU. We've got RAM. We've paging to load up your files into that RAM. YEAAAAHHHHH.

### Disk system emulation
> Oh wow I wish there was a way to store this text in a persistent way
> - Someone

Do you enjoy creating and mounting partitions with custom defined File Allocation Table parameters? Look no more. Use the command `mount PARTITION_NAME NUMBER_OF_BLOCKS BYTES_PER_BLOCK` and jason-os will create a new partition and mount it for you. If it already exists, it'll just mount it for you.

### File system
Now that you've got your brand new partition mounted, what now? Write a sequence of space delimited strings to it using `write FILENAME [WORDS]` (include the brackets). If the file already exists with that name, it'll append your new words. Read from a file to the shell memory with `read FILENAME VAR`.

## Sounds great, how do I run this?
This assumes that you're running on linux. After you've cloned the repo, simply run `make` to build it, and run jason-os with `./mykernel`!

## Can I copy this?
Feel free to use this in any way you'd like, as long as it's not being used for profit (not sure why you'd want to even do so). If you're also taking this OS course, I'd suggest you give it a shot in making your own project!