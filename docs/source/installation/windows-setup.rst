=============
Windows Setup
=============


.. note::
	Xentu SDK will run on versions of Windows as old as Windows XP given the correct
	setup. Honestly though if you do this, you are mad :)



.. _windows-setup-standard:

Windows Installer
-----------------

If you wish to install the SDK the most traditional way, you can download the
installer using the following link: `XentuSDK_Alpha_Setup_x86.exe <//files.xentu.net/win/XentuSDK_Alpha_Setup_x86.exe>`_

.. _windows-setup-choco:

.. raw:: html

    <!--

Chocolatey Package Manager
--------------------------

Although you can just use the installer above. We reccommend using a package
manager where possible, as this will make it easier to keep the engine up-to
date. To install with Chocolaty (`download here <https://chocolatey.org/>`_)
then enter the following command:

.. code-block:: shell

    choco install xentu

.. raw:: html
    
    -->

.. _windows-setup-troubleshooting:

Make Sure It Works! (Windows)
-----------------------------

Open a command line console or terminal, and enter the following command:

.. code-block:: shell

    xentusdk version

If all goes well you should get a message telling you which version you have
installed. However if it throws an error, it maybe that the sdk is not yet
recognised. Often the easiest way to resolve this is to restart your computer.

If that does not work, then the next thing you can try is to add the path to the
installed SDK into your PATH system environment variable (press start and type 
"system environment variable"). After locating and changing the PATH variable,
restart your console/terminal, and it should work.