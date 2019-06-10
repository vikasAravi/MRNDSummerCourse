from django.db import models

# Create your models here.


class todoList(models.Model):
    name = models.CharField(max_length = 256, blank = False, null = False)
    date_added = models.DateTimeField(auto_now_add=True)


class todoItem(models.Model):
    description = models.TextField()
    due_date = models.DateField()
    completed = models.BooleanField()
    todoitems = models.ForeignKey(todoList, on_delete= models.CASCADE)


