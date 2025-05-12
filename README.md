# Container In C

This is a project that aims to do a barebones implementation of containers in
 C. It is not meant for production use, it is merely a pracice project for
 my Systems Programming course.

 If you want to go through the code to get reference from it, or even add
 a new feature or fix any bugs you find, please feel free to open a pull 
 request or to fork this project :-).

 ## Installing and running

 First, you must install the prerequisites for this project. This project 
 uses libcurl to fetch the alpine minirootfs archive in order to have a 
 working root directory within the container.

 On Debian, you can install libcurl with the following command:

```bash
 sudo apt install libcurl4-openssl-dev
```

Then, you must clone the repo in your local Linux environment (this code uses
Linux functionality and has ONLY been tested on Linux systems, run on other 
systems at your own risk!).

```bash
git clone <REPO-url>
```

Then, change directories into the repository folder and build the project using
`make`. The binary named "container" will be generated in the build folder.

```bash
cd container-in-c
make
```

You can run the program by executing the binary with sudo. You can get the 
flags supported by the binary using the `-h` option.

```bash
sudo ./build/container
```