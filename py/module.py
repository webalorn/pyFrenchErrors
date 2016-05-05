class MyError(Exception):
     def __init__(self, value):
         self.value = value
     def __str__(self):
         return "Probleme: "+repr(self.value)
def lancer(a):
    if a <= 0:
        raise MyError(2*2)
    else:
        lancer(a-1)
