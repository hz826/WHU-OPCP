from django.db import models
from django.contrib.auth.models import AbstractUser

class User(AbstractUser):
    description = models.TextField(default='', max_length=100000)

    def __str__(self):
        return self.username
    
    class Meta:
        db_table = 'user_tb'
        verbose_name = 'UserInfo'
        verbose_name_plural = verbose_name


class Contest(models.Model):
    name = models.CharField(max_length=50)
    creator = models.ForeignKey(User, on_delete=models.CASCADE)
    description = models.TextField(default='', max_length=10000)


class FileModel(models.Model):
    filename = models.CharField(max_length=50)
    file = models.FileField(upload_to='uploads/%Y/%m/%d/')

    # class Meta:
    #     db_table = 'files_storage'


class UserInContest(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    contest = models.ForeignKey(Contest, on_delete=models.CASCADE)
    file = models.ForeignKey(FileModel, on_delete=models.CASCADE)


class Submission(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    contest = models.ForeignKey(Contest, on_delete=models.CASCADE)
    file = models.ForeignKey(FileModel, on_delete=models.CASCADE)
    status = models.CharField(max_length=30, default="Waiting")
    score = models.FloatField(default=0)
    detail = models.TextField(default='', max_length=10000000)


class Judge(models.Model):
    submission = models.ForeignKey(Submission, on_delete=models.CASCADE)
    user2 = models.ForeignKey(User, on_delete=models.CASCADE)
    file2 = models.ForeignKey(FileModel, on_delete=models.CASCADE)
    status = models.CharField(max_length=30, default="Waiting")
    score = models.FloatField(default=0)
    detail = models.TextField(default='', max_length=10000000)