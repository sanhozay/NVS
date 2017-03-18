# NVS

NVS is a command line tool for quick searching of Flightgear navigation
data. It is most useful for verifying frequencies as seen by Flightgear,
where published charts may differ. These are available through the moving
map in Flightgear itself, but searching can be cumbersome.

## Examples

Search for a navaid:

    $ nvs pol
    Searching for ILS NDB VOR
    VOR POL   112.10 150nm  1400ft POLE HILL VOR-DME

Search for an airport (headings are true headings):

    $ nvs egnm
    Searching for ILS NDB VOR
    ILS ILBF  110.90  18nm   681ft EGNM-14  138° ILS-cat-I
    ILS ILF   110.90  18nm   681ft EGNM-32  318° ILS-cat-I

Search by name of navaid (fuzzy search)

    $ nvs -f sherburn
    Searching for ILS NDB VOR (including names)
    NDB SBL   323.00  25nm     0ft SHERBURN NDB

Search for VORs and show coordinates:

    $ nvs -vc mct
    Searching for VOR
    VOR MCT  (010.3136N, 123.9883E) 114.30  50nm    36ft MACTAN VOR-DME
    VOR MCT  (053.3569N, 002.2622W) 113.55 130nm   282ft MANCHESTER VOR-DME
    VOR MCT  (023.5911N, 058.2601E) 114.50 130nm    80ft SEEB VOR-DME

Search for NDB and show Morse code ident:

    $ nvs -nm sbl
    Searching for NDB
    NDB SBL   323.00  25nm     0ft SHERBURN NDB ... -... .-..

Search for VORs with bounded coordinates, suppressing messages:

    $ nvs -vqb60,2,50,-4 mct
    VOR MCT   113.55 130nm   282ft MANCHESTER VOR-DME

Search for navaids on a bounded route with wildcard bounds:

    $ nvs -qb60,*,50,* ilf pol hon wco bnn irr
    ILS ILF   110.90  18nm   681ft EGNM-32  318° ILS-cat-I
    VOR POL   112.10 150nm  1400ft POLE HILL VOR-DME
    VOR HON   113.65 130nm   435ft HONILEY VOR-DME
    NDB WCO   335.00  30nm     0ft WESTCOTT NDB
    VOR BNN   113.75 130nm   500ft BOVINGDON VOR-DME
    ILS IRR   110.30  18nm    83ft EGLL-27R 270° ILS-cat-I

Search for all types of navaid (including DME) with spacers:

    $ nvs -sa pol mct
    VOR POL   112.10 150nm  1400ft POLE HILL VOR-DME
    DME POL   112.10 150nm  1400ft POLE HILL VOR-DME
    -
    NDB MCT   331.00  50nm    36ft MACTAN NDB
    VOR MCT   114.30  50nm    36ft MACTAN VOR-DME
    VOR MCT   113.55 130nm   282ft MANCHESTER VOR-DME
    VOR MCT   114.50 130nm    80ft SEEB VOR-DME
    DME MCT   114.30  50nm    36ft MACTAN VOR-DME
    DME MCT   113.55 130nm   282ft MANCHESTER VOR-DME
    DME MCT   114.50 130nm    80ft SEEB VOR-DME
    -

## Building

### Pre-requisites

A working C compiler and build tools including `cmake` and `make`.

The project has a dependency on `zlib`.

GNU/Linux distributions should have a zlib development package, e.g.

    DEB: zlib-1g-dev
    RPM: zlib-devel

On Mac OS, if you have XCode, you should already have `zlib`.

### Setting Up

Clone or download the project.

    $ git clone https://githib.com/sanhozay/NVS.git

Create a build directory inside the project root and run cmake from it.

    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_INSTALL_PREFIX=$HOME -DCMAKE_BUILD_TYPE=Release ..

The default install directory, if no install prefix is supplied is
`/usr/local/bin`.
    
### Building the project

Build the executable in the build directory:

    $ make
    
Install to your install directory:

    $ make install

If you have Doxygen installed, you can also build developer documentation

    $ make doc

Documentation is created in `build/doc`.

## Setup

Define an environment variable FG_ROOT that provides the path to the 
directory containing your fgdata, e.g.

`$ export FG_ROOT=$HOME/Flightgear/fgdata`

You may want to define the variable in your `.profile`, `.bashrc`, etc.

## Usage and options

    Usage: nvs [OPTIONS] ITEMS ...
      -a, --all              Search for all navaid types, including DME
      -b, --bounds=<bounds>  Bounded by [t],[r],[b],[l] (wildcard '*')
      -c, --coordinates      Show coordinates
      -f, --fuzzy            Search names as well as codes
      -h, --help             Show this help message
      -m, --morse            Show Morse code for each navaid
      -q, --quiet            Don't display additional messages
      -s, --spacers          Add spacer lines between results
    Search restrictions:
      -d, --dme              Search for DMEs, including standalone
      -i, --ils              Search for ILS/LOC
      -n, --ndb              Search for NDBs
      -v, --vor              Search for VOR/VORTAC
