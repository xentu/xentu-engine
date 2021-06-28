===================
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