# ToastLib
A plain C library to handle windows toast notifications.

Includes a console and a GUI sample demo.

This library is based on concepts derived from former work of Valentin-Gabriel Radu, but completely rewritten.

The main goal of the library is to hide the whole complex toast notification mechanisms inside library and making available to the user a couple of handles and few functions to register/unregister callback function and to create and destroy a notification manager.

The user must just supply the XML template for the toast in a memory string.

To speed-up compilation, and allow usage also on compilers that doesn't have windows runtime headers avalable, a condensated header is included will all required windows.runtime definitions. Also the runtime libraries, 32 and 64 bits versions, are supplied.

The whole project is made available for **PellesC** compiler and **Visual Studio**, but it shouldn't be difficult to manage under other compilers.

While you're free to use this software it would be nice to know that you found it useful, and, eventually, where you used. Let mi know commenting on PellesC site https://forum.PellesC.de.
