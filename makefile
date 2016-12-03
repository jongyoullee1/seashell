seashell: shell.c
  gcc shell.c -o signal
  
run: shell
  ./shell

clean:
  rm *~
  rm .out
