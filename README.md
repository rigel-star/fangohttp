# fangohttp - Simple Web Server

To build `make all`.

This is example for using fangohttp to host HTML files over LAN.

RULES
1) HTML files should be put on templates folder.
2) CSS, JS files should be put on static folder.

As you can see in main.c, `home(), about(), and projects()`
are some functions which take `httpreq_t*` as argument and
returns `webpage*`. These functions are created in order to
serve HTML files at specific path.

Function path::
```
c path(char*, struct webpage* (*)(httpreq_t*))
```

Available URL paths in application must be added before they
can be accessed. First parameter of path() takes URL path and
second one takes function pointer which handles the behavior of
that URL(home(), about(), and projects() in our example).

For example: `127.0.0.1/about.html` will be handled by home() because
in path() function(main function in main.c), "/about" is given as
first argument to path and about() function is given as second argument.
