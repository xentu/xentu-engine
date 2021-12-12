===================
Linux/FreeBSD Setup
===================

.. _linux-bsd-setup-apt:

APT Package Manager
-------------------

If you are using Ubuntu or a variant like Lubuntu/Mint, you can use the APT Package
manager that is inststalled on your system. First of all we need to add the repo
and update apt so it knows where to find the files, then do the actuall install.
So do this by opening a terminal window, and enter following commands one by one:

.. code-block:: shell

    echo "deb [arch=amd64] https://xentu.net/files/apt stable main" | sudo tee /etc/apt/sources.list.d/xentu.list
    sudo apt-get update
    sudo apt-get install xentu
    
.. _linux-bsd-setup-yum:

YUM Package Manager
-------------------

If you prefer or only have access to use the YUM Package Manager, enter the
following commands one by one instead into your terminal:

.. code-block:: shell

    sudo yum-config-manager --add-repo https://xentu.net/files/yum/xentu.repo
    sudo yum-config-manager --enable xentu
    sudo yum install xentu

.. _linux-bsd-setup-troubleshooting:

Make Sure It Works! (Linux/FreeBSD)
-----------------------------------

Open a command line console or terminal, and enter the following command:

.. code-block:: shell

    xentusdk version

If all goes well you should get a message telling you which version you have
installed.