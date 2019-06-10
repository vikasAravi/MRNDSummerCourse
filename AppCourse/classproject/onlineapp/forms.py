
from django import forms

from .models import *

class studentForm(ModelForm):
    class Meta:
        model = Student
        fields = ['name', 'dob', 'email', 'db_folder', 'college']
