=============
FreeBSD Setup
=============

.. _bsd-setup:

APT Package Manager
-------------------

FreeBSD support is not yet complete, changes need to be made to the pathing in
the SDK to make sure that resources can be located correctly, and a RPM script
needs finishing to allow the installer to work correctly.

.. _bsd-setup-troubleshooting:

Make Sure It Works! (FreeBSD)
-----------------------------------

Open a command line console or terminal, and enter the following command:

.. code-block:: shell

    xentusdk version

If all goes well you should get a message telling you which version you have
installed.