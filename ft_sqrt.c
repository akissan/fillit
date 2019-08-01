int ft_sqrt(int nbr)
{
  int sqrt;

  sqrt = 1;
  while (sqrt * sqrt < nbr)
    sqrt++;
  return (sqrt);
}
