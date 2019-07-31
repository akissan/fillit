for i in range(0, 20):
  print('a' + str(i) + "->grid = " + chr(ord('a') + i) + ';')
  print('a' + str(i) + " = AlignAndCalc(" + 'a' + str(i) + ", cols);");

for i in range(0, 20):
  print("ttrs[" + str(i) + "] = a" + str(i) + ";")

for i in range(0, 20):
  print("t_ttr *a" + str(i) + " = (t_ttr *)malloc(sizeof(t_ttr));")

for i in range(0, 20):
  print("unsigned short "+ chr(ord('a') + i) + ' = grid_fill("1000100010001000");');