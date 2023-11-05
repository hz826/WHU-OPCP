from django.http import HttpResponseRedirect
from django.shortcuts import render
from django.urls import reverse
from django.contrib.auth.models import User
from django.contrib.auth import authenticate
from django.views.generic import ListView, CreateView


def index(request):
    return render(request, 'users/index.html')


def register(request):
    if request.method == 'POST':
        _username = request.POST['username']
        _password = request.POST['password']
        if User.objects.filter(username=_username):
            return render(request, 'users/register.html', {"fail":True})
        user = User.objects.create_user(
            username=_username,
            password=_password
        )
        return HttpResponseRedirect(reverse('users:index'))
    return render(request, 'users/register.html')


def login(request):
    if request.method == 'POST':
        _username = request.POST['username']
        _password = request.POST['password']
        user = authenticate(username=_username, password=_password)
        if user is not None:
            return render(request, 'users/index.html', {"username":_username})
        else:
            return render(request, 'users/login.html', {"fail":True})
    return render(request, 'users/login.html')