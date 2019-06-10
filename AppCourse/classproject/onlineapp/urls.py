from django.contrib import admin
from django.urls import include, path
from django.views.generic import TemplateView

from django.conf import settings
from .import views

urlpatterns = [

    path('hello/', views.hello_world),
    path('studentForm/', views.populate_student_data),
    path('get_my_college/', views.fetch_data),
    path('get_all_colleges/', views.get_all_colleges),
    path('college_student_info/<int:college_id>', views.college_student_info, name = "fetch")
]