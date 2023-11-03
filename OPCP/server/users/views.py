from rest_framework import mixins, generics, status
from rest_framework.views import APIView
from rest_framework.response import Response
from django.contrib.auth import authenticate
from .models import User, Contest, FileModel
from .serializers import UserSerializer, ContestSerializer, FileSerializer


class UserList(APIView):
    '''
        GET:
            Return all objects.
    '''
    def get(self, request):
        users = User.objects.all()
        serializer = UserSerializer(users, many=True)
        return Response(serializer.data)
    

class UserRegister(APIView):
    '''
        POST:
            Register an user.
    '''
    def post(self, request):
        _username = request.data.get('username')
        _password = request.data.get('password')
        serializer = UserSerializer(data=request.data)
        # if User.objects.filter(username=_username):
        #     return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)
        # else:
        if serializer.is_valid():
            User.objects.create_user(
                username=_username,
                password=_password
            )
            return Response(serializer.data)
        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)


class UserLogin(APIView):
    '''
        POST:
            Login
    '''
    def post(self, request):
        _username = request.data.get('username')
        _password = request.data.get('password')
        serializer = UserSerializer(data=request.data)
        user = authenticate(username=_username, password=_password)
        if user is not None:
            data = {"id": user.id}
            return Response(data, status=status.HTTP_200_OK)
        return Response(status=status.HTTP_400_BAD_REQUEST)


class UserDetail(generics.RetrieveUpdateDestroyAPIView):
    '''
        GET:
            Return an object instance.

        PUT:
            Update an object.
        
        DELETE:
            Delete an object.
    '''
    queryset = User.objects.all()
    serializer_class = UserSerializer


class ContestList(generics.ListCreateAPIView):
    queryset = Contest.objects.all()
    serializer_class = ContestSerializer


class ContestDetail(generics.RetrieveUpdateDestroyAPIView):
    queryset = Contest.objects.all()
    serializer_class = ContestSerializer


class FileUpload(generics.CreateAPIView):
    queryser = FileModel.objects.all()
    serializer_class = FileSerializer