from django.shortcuts import render
from .forms import studentForm
# Create your views here.
from django.http import HttpResponse
from .models import *
from django.core import serializers

def hello_world(request):
    # print(request.headers)
    return HttpResponse("Hello")


def fetch_data(request):
    val = College.objects.filter(acronym = 'vit').values('name')
    val = (val[0]['name'])
    return HttpResponse(val)

def get_all_colleges(request):
    val = College.objects.all().values('name', 'acronym', 'id')
    # result = ''
    # for item in range(val.count()):
    #     result = result + val[item]['name'].split()[0] +" " + val[item]['acronym'] + "\n"
    # data = dict()
    # for item in range(val.count()):
    #    data[val[item]['acronym']] = val[item]['name']
    # print(data)
    return render(request, 'index.html', context = {'data' : list(val)})


def college_student_info(request, college_id):
    students = Student.objects.filter(college_id = college_id).values('name', 'id', 'mocktest1__total')
    return render(request, 'getdata.html', context={'data': list(students)})


def populate_student_data(request):
    if request.method == "POST":
        form = studentForm(request.POST)
        name = request.POST['name']
        # dob = request.POST['dob']
        email = request.POST['email']
        db_folder = request.POST['db_folder']
        college_id = request.POST['college']
        object = College.objects.get(id = college_id)
        c = Student(name = name, email = email, db_folder  = db_folder, college = object)
        c.save()
        return HttpResponse(object)
    form = studentForm()
    return render(request, 'studentform.html', {'form': form})

