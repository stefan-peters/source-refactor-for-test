
#this list of paths works for all MY machines. Adapt if needed
PATHS="/opt/local/bin
$HOME/Development/llvm/install/bin
/opt/clang-3.5/bin
/opt/cmake-2.8.12/bin"

for p in `echo $PATHS`; do
	if [[ -e "$p" ]]; then
		echo "adding $p to the PATH"
		export PATH="$p:$PATH"
	fi
done
