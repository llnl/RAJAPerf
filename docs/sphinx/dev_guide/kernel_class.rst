.. ##
.. ## Copyright (c) Lawrence Livermore National Security, LLC and other
.. ## RAJA Project Developers. See top-level LICENSE and COPYRIGHT
.. ## files for dates and other details. No copyright assignment is required
.. ## to contribute to RAJA Performance Suite.
.. ##
.. ## SPDX-License-Identifier: (BSD-3-Clause)
.. ##

.. _kernel_class-label:

====================
Kernel Class Files
====================

Each kernel in the Suite is implemented in a class whose header and 
implementation files reside in the ``src`` subdirectory named for the group in
which the kernel lives. A kernel class is responsible for implementing all 
operations that manage data, execute, and record execution timing, checksums,
and other information for each variant and tuning of a kernel. To properly 
integrate into the RAJA Performance Suite framework, the kernel class must be 
a subclass of the ``KernelBase`` base class that defines the interface for 
kernels in the Suite. The ``KernelBase.hpp`` header file resides in the 
``src/common`` directory.

Continuing with the example we started discussing above, we add the 
``ADD.hpp`` header file for the **ADD** class to the ``stream`` directory 
along with multiple implementation files. We describe the contents of these 
files in the following sections:

  * ``ADD.cpp`` contains methods to set up and tear down the memory for the 
    **ADD** kernel, and compute and record a checksum on the result after 
    it executes. It also specifies **ADD** kernel information in the ``ADD``
    class constructor.
  * ``ADD-Seq.cpp`` contains sequential CPU variants and tunings of the kernel.
  * ``ADD-OMP.cpp`` contains OpenMP CPU multithreading variants and tunings of 
    the kernel.
  * ``ADD-OMPTarget.cpp`` contains OpenMP target offload variants and tunings 
    of the kernel.
  * ``ADD-Cuda.cpp`` contains CUDA GPU variants and tunings of the kernel.
  * ``FOO-Hip.cpp`` contains HIP GPU variants and tunings of the kernel.
  * ``FOO-Sycl.cpp`` contains SYCL variants and tunings of the kernel.

.. note:: All kernels in the Suite follow the same file organization and
          implementation pattern. Inspection of the files for any individual
          kernel will help to understand how the kernel class implementations
          are organized.

.. important:: If a new execution back-end variant is added that is not listed 
               here, that variant should be placed in a file named to clearly
               distinguish the back-end implementation, such as 
               ``ADD-<back-end>.cpp``. Keeping the variants for each back-end 
               in a separate file helps to understand compiler optimization
               when looking at generated assembly code, for example, and also
               to work with vendors on issues.

.. _kernel_class_header-label:

-------------------------
Kernel class header file
-------------------------

In its entirety, the **ADD** kernel class header file ``ADD.hpp`` is:

.. literalinclude:: ../../../src/stream/ADD.hpp
   :language: C++

The key parts of a kernel class header file are:

  * **Copyright statement** at the top of the file.

    .. note:: Each file in the RAJA Performance Suite must start with a 
              boilerplate comment for the project copyright information.

  * **Reference implementation**, which is a comment section that shows a
    C-style implementation of the kernel. This is basically what the
    ``Base_Seq`` variant of the kernel looks like. All kernel variants 
    should produce results close to the reference version.

  * **Uniquely-named include guard** that guards the contents of 
    the header file. All such guards have the form 
    ``RAJAPerf_<group name>_<kernel name>_HPP``.

  * **Macro definitions** that contain source lines of code that appear in 
    multiple places in the kernel class implementation, such as setting 
    data pointers and operations in the kernel body. While macros obfuscate
    the code somewhat, we use them to reduce the amount of code we maintain 
    and ensure implementations are consistent across variants.

  * **Class definition** derived from the ``KernelBase`` class. We describe
    this in more detail below.

.. note:: * All types, methods, etc. in the RAJA Performance Suite reside in 
            the ``rajaperf`` namespace. 
          * In addition, each kernel class lives in the namespace of the 
            kernel group of which the kernel is a member. For example, 
            here, the ``ADD`` class is in the ``stream`` namespace.
          * Each kernel class **must be publicly derived** from the
            ``KernelBase`` class so that the kernel integrates properly into
            the Suite.

The class must provide a constructor that takes a reference to a ``RunParams`` 
object, which contains input parameters for running the Suite -- we'll say more 
about this later. The class constructor may or may not allocate storage for
a class object. If it does, the storage should be deallocated in the class 
destructor.

Several methods in the ``KernelBase`` class are pure virtual and the derived
kernel class must provide implementations of those methods. These methods
take a ``VariantID`` argument and a tuning index of type ``size_t``. They
include: ``setUp``, ``updateChecksum``, and ``tearDown``.

Other methods in the code above, such as ``defineCudaVariantTunings`` are
virtual in the ``KernelBase`` class and so they may be provided optionally by 
the kernel class for kernel specific operations. The ``define*VariantTunings``
methods specify which variants are implemented and define the tunings that are
available for the kernel.

Some other methods in the code above, such as ``runSeqVariant`` and
``runCudaVariantImpl`` are unique to each kernel but the names are expected
by the boilerplate macros used in the kernel source files.

While all method names should be reasonably descriptive of what they do, we'll
provide more details about them when we describe the kernel class
implementation in the next section.

Lastly, any data members used in the class implementation are defined, 
typically in a ``private`` member section so they don't *bleed* out of the
kernel class. For example, in the **ADD** class, we see data members for
default GPU block sizes and a list for holding a set of block sizes for
exploring kernel performance with respect to changes in GPU block size. Also,
there are pointer member variables to hold data arrays for the kernel. Here we
have ``m_a``, m_b``, and ``m_c`` for the three arrays used in the ADD kernel.
Note that we use the convention to prefix class data members with ``m_`` so it
is clear in the source code which data are class members and which are local
variables.
