## ProducerConsumer implementation for Smartmatic Quiz

The two sorting algos chosen are lookup-sort and insertion-sort.

Lookup-sort is the fastest (linear time) in this case and it could be done even in line while feeding vector<char> for each item,
however the quiz was asking to first load the item into vector then sort it so this is the way it's implementated O(2N). The reason
this sorting method can be used is due to the small binary space of char is only 256 and in this case actually due the additional
restriction that the characters are numerical only 10 different values are possible and this is ideal for this sorting method.

Insertion sorting is an alternative way but it's slightly slower O(NlogN) although by not much, other sorting methods are not necessary
again for the same reason that only 10 different values are possible.


## INSTALLATION

$mkdir build

$cd build

$cmake ..

$make

## USAGE

$./ProducerConsumer ~/input.txt ~/output.txt INSERT

$./ProducerConsumer ~/input.txt ~/output.txt LOOKUP

