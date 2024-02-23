
# colconv

a tiny tool to convert colors between the three major representations and their inverses

## examples

### convert hex value
```
% echo "#aabbcc" | colconv.pl
0.667, 0.733, 0.800 / 0.333, 0.267, 0.200
170, 187, 204 / 85, 68, 50
#AABBCC / #554432
```

### convert float value
```
% echo "0.1,0.5,0.5" | colconv.pl
0.100, 0.500, 0.500 / 0.900, 0.500, 0.500
25, 127, 127 / 229, 127, 127
#197F7F / #E57F7F
```

### convert decimal value
```
% echo "80,155,77" | colconv.pl
0.314, 0.608, 0.302 / 0.686, 0.392, 0.698
80, 155, 77 / 175, 100, 178
#509B4D / #AF64B2
```