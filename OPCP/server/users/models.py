from django.db import models
from django.contrib.auth import get_user_model


User = get_user_model()


class Contest(models.Model):
    name = models.CharField(max_length=50)
    creator = models.ForeignKey(User, on_delete=models.CASCADE)
    description = models.TextField(default='', max_length=10000)


class FileModel(models.Model):
    filename = models.CharField(max_length=50)
    file = models.FileField(upload_to='uploads/%Y/%m/%d/')

    # class Meta:
    #     db_table = 'files_storage'


class Submission(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    contest = models.ForeignKey(Contest, on_delete=models.CASCADE)
    file = models.ForeignKey(FileModel, on_delete=models.CASCADE)
    status = models.CharField(max_length=30, default="Waiting")
    score = models.FloatField(default=0)
