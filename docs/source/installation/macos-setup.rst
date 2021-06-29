===========
MacOS Setup
===========

.. warning::
	Support for MacOS is experimental at the moment. Builds have been working,
	however your mialage may vary as we finalize testing.

To run the SDK on MacOS you will need the Brew package manager. We reccommend
using MacOS Mojave or later for the best compatibility. The SDK also works with
Apple Silicon mac's too.

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

    xentusdk version

If all goes well you should get a message telling you which version you have
installed.