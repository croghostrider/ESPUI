#!/usr/bin/env python3

from jsmin import jsmin as jsminify
from htmlmin import minify as htmlminify
from csscompressor import compress as cssminify
import gzip
import sys
import os.path
import argparse
import re
from glob import glob

TARGET_TEMPLATE = '''const char {constant}[] PROGMEM = R"=====(
{minidata}
)=====";

const uint8_t {constant}_GZIP[{gziplen}] PROGMEM = {{ {gzipdata} }};
'''

def parse_arguments(args=None):
    parser = argparse.ArgumentParser(
        description="Prepares ESPUI header files by minifying and gzipping HTML, JS and CSS source files.")
    parser.add_argument("--auto", "--all", "-a", dest="auto", action="store_true",
                        help="Automatically find all source files in data/ and write C header files to src/")
    parser.add_argument("--source", "--sources", "-s", dest="sources", default=None,
                        help="Sources directory containing CSS or JS files OR one specific file to minify")
    parser.add_argument("--target", "-t", dest="target", default=None,
                        help="Target directory containing C header files OR one C header file")
    parser.add_argument("--nostoremini", "-m", action="store_false", dest="storemini",
                        help="Do not store intermediate minified files next to the originals (i.e. only write to the C header files)")
    args = parser.parse_args(args)
    if not args.auto and (not args.sources or not args.target):
        print("ERROR: You need to specify either --auto or both --source and --target\n")
        parser.print_help()
        sys.exit(1)
    return args

def get_context(infile, outfile):
    infile	= os.path.realpath(infile)
    dir, name, type 	= (os.path.basename(os.path.dirname(infile)), os.path.basename(infile).split(os.path.extsep)[0], os.path.basename(infile).split(os.path.extsep)[-1] )
    type = type.strip(".").lower()
    if dir.lower() == type:
        dir = os.path.basename(os.path.dirname(os.path.dirname(infile)))
    if type == "htm":
        type = 'html'
    name = os.path.basename(name)
    indir	= os.path.dirname(infile)
    if os.path.isdir(outfile):
        outdir 	= os.path.realpath(outfile)
        outfilename = f"{dir}{name.capitalize()}{type.upper()}.h"
        outfile	= os.path.realpath(os.path.sep.join([outdir, outfilename]))
    else:
        outfile	= os.path.realpath(outfile)
        outdir	= os.path.dirname(outfile)
        outfilename	= os.path.basename(outfile)
    minifile = (
        re.sub('\.([^.]+)$', '.min.\\1', infile)
        if ".min." not in infile
        else infile
    )
    constant = f'{type.upper()}_{name.upper()}'
    return locals()
    
def perform_gzip(c):
    compressed = gzip.compress(bytes(c['minidata'], 'utf-8'))
    c['gzipdata'] = ','.join([ str(b) for b in compressed ])
    c['gziplen'] = len(compressed)
    print(f"  GZIP data length: {c['gziplen']}")
    return c
    
def perform_minify(c):
    with open(c['infile']) as infile:
        minifier = {
            'css': cssminify, 
            'js': jsminify, 
            'html': htmlminify
        }.get(c['type']) or htmlminify
        print(f"  Using {c['type']} minifier")
        c['minidata'] = minifier(infile.read())
    return perform_gzip(c)

def process_file(infile, outdir, storemini=True):
    print(f"Processing file {infile}")
    c = get_context(infile, outdir)
    c = perform_minify(c)
    if storemini:
        if c['infile'] == c['minifile']:
            print("  Original file is already minified, refusing to overwrite it")
        else:
            print(f"  Writing minified file {c['minifile']}")
            with open(c['minifile'], 'w+') as minifile:
                minifile.write(c['minidata'])
    with open(c['outfile'], 'w+') as outfile:
        print(f"  Using C constant names {c['constant']} and {c['constant']}_GZIP")
        print(f"  Writing C header file {c['outfile']}")
        outfile.write(TARGET_TEMPLATE.format(**c))
        
def filenamefilter(pattern, strings):
    return filter(re.compile(pattern).search, strings)        
        
def process_dir(sourcedir, outdir, recursive=True, storemini=True):
    pattern = r'/*\.(css|js|htm|html)$'
    files = (
        glob(f"{sourcedir}/**/*", recursive=True) + glob(f"{sourcedir}/*")
        if recursive
        else glob(f"{sourcedir}/*")
    )
    files = filenamefilter(pattern, files)
    for f in set(files):
        if '.min.' not in f:
            process_file(f, outdir, storemini)
        elif not os.path.isfile(f.replace(".min.", ".")):
            process_file(f, outdir, storemini)
            
def check_args(args):
    abort = 0
    if not os.path.exists(args.sources):
        print(f"ERROR: Source {args.sources} does not exist")
        abort += 2
    if not os.path.isdir(os.path.dirname(args.target)):
        print(f"ERROR: Parent directory of target {args.target} does not exist")
        abort += 4
    if os.path.isdir(args.sources) and not os.path.isdir(args.target):
        print(
            f"ERROR: Source {args.sources} is a directory, target {args.target} is not"
        )
        abort += 8
    if abort > 0:
        print("Aborting.")
        sys.exit(abort)

def main(args):
    args.sources = os.path.realpath(args.sources or os.sep.join((os.path.dirname(os.path.realpath(__file__)), "..", "data")))
    args.target  = os.path.realpath(args.target  or os.sep.join((os.path.dirname(os.path.realpath(__file__)), "..", "src")))
    check_args(args)
    if os.path.isfile(args.sources):
        print(f"Source {args.sources} is a file, will process one file only.")
        process_file(args.sources, args.target, storemini = args.storemini)
    elif os.path.isdir(args.sources):
        print(
            f"Source {args.sources} is a directory, searching for files recursively..."
        )
        process_dir(args.sources, args.target, recursive = True, storemini = args.storemini)

if __name__ == "__main__" and "get_ipython" not in dir():
    main(parse_arguments())
