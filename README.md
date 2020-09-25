# Kernel_callbacks
 
Just playing with internal windows memory to get the process, image, Thread, Regs callbacks by using winpmem driver.
Using MemProcFS to communicate with winpmem driver to read kernel space in the live system. The tricky part is to map the non-export kernel variable in memory, thanks to the Mimidrv code that helps me find these variables.
The result of this tool will print you the process creation, load-image, Thread created/deleted, and registry callbacks address with owner-drivers.
