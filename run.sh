SOURCE="p1.c"
EXECUTABLE="p1"
FLAGS="-std=gnu99 -Wall -Wextra -pedantic"

echo "COMPILATION ---------------------------------------"
rm $EXECUTABLE
gcc $FLAGS $SOURCE -o $EXECUTABLE -fopenmp
echo "---------------------------------------------------"

./$EXECUTABLE