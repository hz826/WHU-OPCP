from django.urls import path
from rest_framework.urlpatterns import format_suffix_patterns
from rest_framework_simplejwt.views import TokenObtainPairView, TokenRefreshView
from . import views


urlpatterns = [
    path('users/', views.UserList.as_view()),
    path('register/', views.UserRegister.as_view()),
    path('login/', views.UserLogin.as_view()),
    path('users/<int:pk>', views.UserDetail.as_view()),

    path('contests/', views.ContestList.as_view()),
    path('contests/<int:pk>', views.ContestDetail.as_view()),

    path('file/', views.FileList.as_view()),
    path('upload/', views.FileUpload.as_view()),
    path('download/<int:pk>', views.FileDownload.as_view()),

    path('token/', TokenObtainPairView.as_view()),
    path('token/refresh', TokenRefreshView.as_view()),

    path('submissions/', views.SubmissionList.as_view()),
    path('submissions/<int:pk>', views.SubmissionDetail.as_view()),
    path('submit/', views.Submit.as_view()),
    path('getjudge/<int:pk>', views.GetJudgeOfSubmission.as_view()),
    
    path('gettask/', views.GetTask.as_view()),
    path('judges/', views.JudgeList.as_view()),
    path('judges/<int:pk>', views.JudgeDetail.as_view()),

    path('contests/<int:pk>/ranklist/', views.Ranklist.as_view()),
    path('getscore/', views.GetScore.as_view()),
    path('updatescore/', views.UpdateScore.as_view()),
]   


urlpatterns = format_suffix_patterns(urlpatterns)