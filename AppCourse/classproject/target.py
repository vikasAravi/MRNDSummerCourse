import os
import django
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'classproject.settings')
django.setup()


from onlineapp.models import *
test = College.objects.all().order_by('acronym')
c = College.objects.all().order_by('-acronym')[:5]
print(test)
print(c)