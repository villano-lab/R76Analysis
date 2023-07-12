# Uses for Singularity
Singularity is a software for running containers, akin to docker. 
It is therefore useful for sharing environments that are difficult to recreate.
CDMS uses many custom libraries that are difficult or time-consuming to install,
so the use of a Singularity container can be a big time-saver.

# Requirements
Singularity's requirements are listed [in their installation instructions](https://github.com/sylabs/singularity/blob/main/INSTALL.md#install-system-dependencies). 
Note that the system dependencies are already installed on cdms2. 
You will still need to install go, which we will get to in a moment.

**BEFORE YOU INSTALL GO ON CDMS2**, you will want to use the following commands to alter your environment. 
These changes work around the storage quotas on cdms2, as the caches that go builds are very large.
You may wish to add these to your .bashrc as well. 
Replace `yourdirectory` with a directory you have made for yourself on `/data/chocula/`, 
and make sure to create the directories specified.
* `export GOCACHE="/data/chocula/yourdirectory/.cache/go"`
* `export GOMODCACHE="/data/chocula/yourdirectory/.cache/go/pkg/mod"`
* `export GOPATH="/data/chocula/yourdirectory/go"`
* `export CCACHE_DIR="/data/chocula/yourdirectory/.ccache"`
You may now install go following [their instructions here](https://go.dev/doc/install).

# Installation
Once you have installed go, you can begin installing Singularity somewhere inside `/data/chocula/yourdirectory`.
Note that the `--recurse-submodules` flag on `git checkout` does not work on cdms2 because the git version is older than the flag.
Therefore, we will alter the first few steps:
1. `git clone --recurse-submodules https://github.com/sylabs/singularity.git`
2. `cd singularity`
3. `git checkout v3.11.4`
4. `git submodule update --init --recursive`

Once you have done this, you should be able to follow [the compilation instructions](git checkout --recurse-submodules v3.11.4).

# Getting the CDMS Singularity Image

The easiest way to get the CDMS singularity image on CDMS2 is to copy mine:

`cp /data/chocula/harrisk/cdmsfull_latest.sif /your/path/here`

However, if you are not on CDMS2, the necessary steps are [on the SLAC wiki here](https://confluence.slac.stanford.edu/display/CDMS/Using+CDMS+Offline+Software+Releases#UsingCDMSOfflineSoftwareReleases-Singularityimage). You will need access to the CDMS GitLab group(s) for this to work.

# Running Jupyter from the Singularity Image

Now that you have the singularity image, you can run it, with the `/data/` directory mounted, using:

`singularity run /path/to/cdmsfull_latest.sif -B /data --no-browser --port=XXXX`

Where XXXX is whatever port you want to run Jupyter on (the same one that you forward via SSH, if you are using SSH).
Once you are in, make sure that you are using the default `Python 3` kernel.
You should now have the full CDMS python suite available to you!

*Last updated 12/July/2023.*