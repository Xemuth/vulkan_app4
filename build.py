import os
import subprocess
import argparse
import shutil
from re import sub

def camel_case(s):
  s = sub(r"(_|-)+", " ", s).title().replace(" ", "")
  return ''.join([s[0].lower(), s[1:]])

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Build options")
    parser.add_argument("-b", "--build",
                        help="Select build type", choices=["release", "debug"], required=False, action="store", default="debug", type=str.lower)
    parser.add_argument("-r", "--reconfigure",
                        help="Reconfigure cmake project", required=False, action="store_true", default=False)
    parser.add_argument("-g", "--generateur",
                        help="Select generator", choices=["ninja", "msvc"], required=False, action="store", default="ninja", type=str.lower)
    parser.add_argument("-e", "--execute",
                        help="Execute the generated binary", required=False, action="store_true", default=False)
    parser.add_argument("--logger",
                        help="Enable logger", required=False, action="store_true", default=False)
    parser.add_argument("--validation_layer",
                        help="Enable validation layer", required=False, action="store_true", default=False)
    args = parser.parse_args()

    if not os.path.exists("CMakeLists.txt"):
        print("can't find MakeLists.txt")
        exit(-1)
    try:
        generate_cmd = ["cmake", "-B", "out"]
        if args.generateur == "ninja":
            generate_cmd = generate_cmd + ["-G", "Ninja"]
        elif args.generateur == "msvc":
            generate_cmd = generate_cmd + ["-G", "MSVC"]

        if args.build is not None:
            generate_cmd = generate_cmd + ["-DCMAKE_BUILD_TYPE=" + camel_case(args.build)]

        if args.validation_layer is not None and args.validation_layer:
            generate_cmd = generate_cmd + ["-DENABLE_VALIDATION_LAYER=ON"]

        if args.logger is not None and args.logger:
            generate_cmd = generate_cmd + ["-DENABLE_LOGGER=ON"]
        
        if args.reconfigure and os.path.exists("./out"):
            shutil.rmtree("./out")

        subprocess.check_call(generate_cmd)
        subprocess.check_call(["cmake", "--build", "./out"])
        if args.execute:
            path_to_exe = "./bin/" + args.build + "/vulkan_app4.exe"
            subprocess.check_call([path_to_exe], cwd="./bin/" + args.build + "/")
        exit(0)
    except Exception as e:
        print("Error: "+  str(e))
        exit(-1)
