# fangohttp - Simple Web Server

To build `make all`.

/**\n
** This is example for using fangohttp to host HTML files over LAN.\n
**\n
** RULES\n
** 1) HTML files should be put on templates folder.\n
** 2) CSS, JS files should be put on static folder.\n
**\n
** As you can see in main.c, home(), about(), and projects()\n
** are some functions which take httpreq_t* as argument and\n
** returns webpage*. These functions are created in order to\n
** serve HTML files at specific path.\n
**\n
** Function path:: path(char*, struct webpage* (*)(httpreq_t*))\n
** Available URL paths in application must be added before they\n
** can be accessed. First parameter of path() takes URL path and\n
** second one takes function pointer which handles the behavior of\n
** that URL(home(), about(), and projects() in our example).\n
**\n
** For example: 127.0.0.1/about.html will be handled by home() because\n
** in path() function(main function in main.c), "/about" is given as\n
** first argument to path and about() function is given as second argument.\n
**/\n
