# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.main.Debug:
/Users/chuxiaomin/Develop/Gomoku/GomokuZero/xbuild/bin/Debug/main:
	/bin/rm -f /Users/chuxiaomin/Develop/Gomoku/GomokuZero/xbuild/bin/Debug/main


PostBuild.main.Release:
/Users/chuxiaomin/Develop/Gomoku/GomokuZero/xbuild/bin/Release/main:
	/bin/rm -f /Users/chuxiaomin/Develop/Gomoku/GomokuZero/xbuild/bin/Release/main


PostBuild.main.MinSizeRel:
/Users/chuxiaomin/Develop/Gomoku/GomokuZero/xbuild/bin/MinSizeRel/main:
	/bin/rm -f /Users/chuxiaomin/Develop/Gomoku/GomokuZero/xbuild/bin/MinSizeRel/main


PostBuild.main.RelWithDebInfo:
/Users/chuxiaomin/Develop/Gomoku/GomokuZero/xbuild/bin/RelWithDebInfo/main:
	/bin/rm -f /Users/chuxiaomin/Develop/Gomoku/GomokuZero/xbuild/bin/RelWithDebInfo/main




# For each target create a dummy ruleso the target does not have to exist
