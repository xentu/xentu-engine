===============
Create A Folder
===============

Whenever you start work on a project in Xentu, you should create a folder to
host the files within. Once you have done that, use your terminal/console to
navigate to it's location.

New to the console?
-------------------

Say we are in Windows. When you load up a console like the Windows Command Prompt,
the location you start in is commonly your user folder (``c:\\Users\\conta``). As
the documents (aka My Documents) folder is within this, lets enter some commands
to create a folder.

1. cd (change directory) into the documents folder:

.. code-block:: shell

	cd documents

2. Make a new directory for our project:

.. code-block:: shell

	mkdir my_awesome_game

3. cd (change directory) into the new directory we created:

.. code-block:: shell

	cd my_awesome_game

4. As a shortcut, next time you launch the console. Typing the following will get you back to the directory:

.. code-block:: shell

	cd documents/my_awesome_game

You can now use the SDK commands you'll see further down on this article!

.. admonition:: Other Operating Systems

	The commands above generally work on other operating systems like Linux/MacOS
	etc... however the directory you start on will usually be different. Instead
	of documents, we reccommend putting your work under ``/home/xentu/[project_name]``
	instead.    