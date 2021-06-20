============
Installation
============

Creating your first game using Xentu is very easy, and can be done on nearly any
modern computer. To get started you will need a copy of the Xentu SDK (Software
Development Kit) installed on your computer.

The SDK is a small program that enables tools that can be accessed using your
chosen command line console or terminal. The tools provided include: creating 
new game projects, copying the latest binaries to your exisiting game folder,
debugging problems you may encounter, and packaging your game so that you can
share with others.

So let's get stuck in! Pick the operating system you are using below, and follow
the instructions to get started;

|

------------

|

.. _windows-setup:

Windows SDK Setup
=================

Xentu SDK will run on versions of Windows as old as Windows 7 given the correct
setup. However as it is end of life, and Windows 8 also nearing it's end of life
for security updates, we encourage you to use an as modern version of Windows as
possible.

.. _windows-setup-standard:

Windows Installer
-----------------

If you wish to install the SDK the most traditional way, you can download the
installer using the following link: `XentuSDK_Setup_x86.exe <//xentu.net/downloads/latest/x86/XentuSDK_Setup_x86.exe>`_

.. _windows-setup-choco:

Chocolatey Package Manager
--------------------------

Although you can just use the installer above. We reccommend using a package
manager where possible, as this will make it easier to keep the engine up-to
date. To install with Chocolaty (`download here <https://chocolatey.org/>`_)
then enter the following command:

.. code-block:: shell

    choco install xentu

.. _windows-setup-troubleshooting:

Make Sure It Works! (Windows)
-----------------------------

Open a command line console or terminal, and enter the following command:

.. code-block:: shell

    xentu-sdk --version

If all goes well you should get a message telling you which version you have
installed. However if it throws an error, it maybe that the sdk is not yet
recognised. Often the easiest way to resolve this is to restart your computer.

If that does not work, then the next thing you can try is to add the path to the
installed SDK into your PATH system environment variable (press start and type 
"system environment variable"). After locating and changing the PATH variable,
restart your console/terminal, and it should work.

|

------------

|

.. _linux-bsd-setup:

Linux/BSD SDK Setup
===================

.. _linux-bsd-setup-apt:

APT Package Manager
-------------------

If you are using Ubuntu or a variant like Lubuntu/Mint, you will likely already
have APT inststalled on your system. So to install Xentu SDK using APT, Open a
terminal window, and enter following command:

.. code-block:: shell

    apt-get install xentu

.. _linux-bsd-setup-yum:

YUM Package Manager
-------------------

If you prefer to use the YUM Package Manager, enter the following command in
your terminal instead:

.. code-block:: shell

    yum install xentu

.. _linux-bsd-setup-troubleshooting:

Make Sure It Works! (Linux/BSD)
-------------------------------

Open a command line console or terminal, and enter the following command:

.. code-block:: shell

    xentu-sdk --version

If all goes well you should get a message telling you which version you have
installed.

|

------------

|

.. _macos-setup:

MacOS Setup
===========

Support for MacOS is experimental at the moment. However we do have builds for
the Brew package manager available for you to try. We reccommend using MacOS
Mojave or later for the best compatibility. Xentu SDK does work with Apple
Silicon mac's too.

.. _macos-setup-brew:

Brew Package Manager
--------------------

If you do not have Brew installed already, visit `this link <https://brew.sh/>`_
to download and install. Once done, enter the following command in a terminal
window:

.. code-block:: shell

    brew install xentu

.. _macos-setup-troubleshooting:

Make Sure It Works! (MacOS)
---------------------------

Open a command line console or terminal, and enter the following command:

.. code-block:: shell

    xentu-sdk --version

If all goes well you should get a message telling you which version you have
installed.

|

------------

|

.. _chromeos-setup:

ChromeOS Setup
==============

Coming Soon

|

------------

|

.. _haikuos-setup:

Haiku OS Setup
==============

To install the Xentu SDK on Haiku OS, launch HaikuDepot from your applications
menu, and search for "Xentu SDK". Once found, click on it in the list, then
press the Install button.

.. _haikuos-setup-troubleshooting:

Make Sure It Works! (HaikuOS)
-----------------------------

Open a command line console or terminal, and enter the following command:

.. code-block:: shell

    xentu-sdk --version

If all goes well you should get a message telling you which version you have
installed.