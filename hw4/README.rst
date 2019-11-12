======================
Homework assignment #4
======================

Your submission PR needs to create a sub-directory using exactly your GitHub
username as your working directory (``nsdhw_19au/hw4/$username/``).  **You also
need to submit your PR link on E3.** The PR needs to be created against the
``hw4`` branch.  (Not ``master``!)

Working directory hierarchy may look like::

  - (nsdhw_19au)
    - (hw4)
      - username (<- your workind directory)
        - Makefile
        - xxx.{cc|cpp|cxx}
        - (any other files you need)

Everyone should write his/her own code.  It is OK to discuss, but there should
not be duplicated code.  If duplication is found, **all points** for the
duplicated part of the latter submitter may be deducted.

Please make sure your PR is compatible with the latest NSD AMI on AWS (Ubuntu
18.04 LTS).  You are not required to use the AMI for doing the homework, but
the grader will use the AMI.  If your code doesn't build or properly run, you
**may not get any point**.

Question
========

Based on what you wrote in the previous homework assignment or that in the
course notes, use tiling technique to speed up the matrix-matrix multiplication
without limiting the matrix or tiling size.  The speed-up must be more than 20%
(runtime must be less than 80%).

Grading guideline
=================

As usual, this homework assignment has 5 points.  On the latest NSD AMI, the
grader will run the following commands:

.. code-block:: bash

  cd nsdhw_19au/hw4/username
  ../validate.py | grep "GET POINTS"

You will earn points for what is printed on screen.  The grader will run on a
`m5.large` in `us-east-1`.  Please test your code in the same or a similar
environment.

The grader may attempt to run the above commands with the environment variable
``PRELOAD_MKL`` set, if ``validate.py`` doesn't pass without it.  Your work is
considered pass in either case.

.. vim: set ft=rst ff=unix fenc=utf8 et sw=2 ts=2 sts=2:
