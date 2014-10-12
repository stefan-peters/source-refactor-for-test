
#this list of paths works for all MY machines. Adapt if needed
PATHS="/opt/local/bin
$HOME/Development/llvm/install/bin
/opt/clang-3.5/bin
/opt/cmake-2.8.12/bin"

PYPATHS="$HOME/Development/llvm/tools/clang/bindings/python"

LIBPATHS="$HOME/Development/llvm/install/lib"

function add_to_env() {

	for p in `echo $2`; do
		if [[ -e "$p" ]]; then
			echo "adding $p to $1"
			export $1="$p:${!1}"
		fi
	done
}

add_to_env "PATH" "$PATHS"
add_to_env "PYTHONPATH" "$PYPATHS"
add_to_env "LD_LIBRARY_PATH" "$LIBPATHS"