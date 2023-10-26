from django.urls import path
from rest_framework.urlpatterns import format_suffix_patterns
from . import views


urlpatterns = [
    path('users/', views.UserList.as_view()),
    path('register/', views.UserRegister.as_view()),
    path('login/', views.UserLogin.as_view()),
    path('users/<int:pk>', views.UserDetail.as_view()),
    path('contests/', views.ContestList.as_view()),
    path('contests/<int:pk>', views.ContestDetail.as_view()),
]   


urlpatterns = format_suffix_patterns(urlpatterns)