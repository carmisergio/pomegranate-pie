# def binaryPow(b, e):
#   let y = 1;
#   while (e > 0) {
#     if (e & 1) y *= b
#     b *= b
#     e >>= 1
#   }
#   return y

def binary_pow(b, e):
    y = 1
    while e > 0:
        if e & 1:
            y *= b
        b*=b
        e >>= 1
    
    return y
        

print(binary_pow(2, 5))
print(binary_pow(12, 2))
print(binary_pow(8, 7))