# ToastLib
A plain C library to handle windows toast notifications.

Includes a console and a GUI sample demo.

This library is based on concepts derived from former work of Valentin-Gabriel Radu, but completely rewritten.

The main goal of the library is to hide the whole complex toast notification mechanisms inside library and making available to the user a couple of handles and few functions to register/unregister callback function and other 2 functions to create and destroy a notification manager.

The user must just supply the XML template for the toast in a memory string.

To speed-up compilation, and allow usage also un compilers that doesn't have windows runtime headers avalable, a condensated header is included will all required windows.runtime definitions.

The whole project is made available for **PellesC** compiler, but it shouldn't be difficult to manage under other compilers.
