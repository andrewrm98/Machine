echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
sudo touch /etc/sysctl.d/01-disable-aslr.conf
echo "kernel.randomize_va_space = 0" | sudo tee /etc/sysctl.d/01-disable-aslr.conf

