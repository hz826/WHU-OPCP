from django.db import models
from django.contrib.auth import get_user_model


User = get_user_model()


class Contest(models.Model):
    name = models.CharField(max_length=50)
    creator = models.ForeignKey(User, on_delete=models.CASCADE)