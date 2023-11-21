# 1. Write a script gen.sh which contains only one line as follows:
$for (( i=0; i<$1; i++ )) ; do echo $RANDOM >> $2; done