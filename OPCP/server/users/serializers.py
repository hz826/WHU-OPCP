from rest_framework import serializers
from .models import User, Contest, FileModel, Submission


class UserSerializer(serializers.ModelSerializer):
    class Meta:
        model = User
        fields = ('id', 'username', 'password')


class ContestSerializer(serializers.ModelSerializer):
    class Meta:
        model = Contest
        fields = '__all__'


class FileSerializer(serializers.ModelSerializer):
    class Meta:
        model = FileModel
        fields = '__all__'


class SubmissionSerializer(serializers.ModelSerializer):
    class Meta:
        model = Submission
        fields = '__all__'