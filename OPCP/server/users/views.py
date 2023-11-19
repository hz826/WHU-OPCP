from rest_framework import mixins, generics, status
from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework.permissions import IsAuthenticated, IsAuthenticatedOrReadOnly
from django.http import Http404, FileResponse
from django.contrib.auth import authenticate
from .models import User, Contest, FileModel, Submission, Judge, UserInContest
from .serializers import UserSerializer, ContestSerializer, FileSerializer, SubmissionSerializer, JudgeSerializer

class UserList(APIView):
    '''
        GET:
            Return all objects.
    '''
    # authentication_classes = [JSONWebTokenAuthentication]
    # permission_classes = []
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
    # permission_classes = [IsAuthenticatedOrReadOnly]
    queryset = User.objects.all()
    serializer_class = UserSerializer


class ContestList(generics.ListCreateAPIView):
    # permission_classes = [IsAuthenticatedOrReadOnly]
    queryset = Contest.objects.all()
    serializer_class = ContestSerializer


class ContestDetail(generics.RetrieveUpdateDestroyAPIView):
    # permission_classes = [IsAuthenticatedOrReadOnly]
    queryset = Contest.objects.all()
    serializer_class = ContestSerializer


class FileList(generics.ListAPIView):
    queryset = FileModel.objects.all()
    serializer_class = FileSerializer


class FileUpload(generics.CreateAPIView):
    # permission_classes = [IsAuthenticated]
    queryset = FileModel.objects.all()
    serializer_class = FileSerializer


class FileDownload(APIView):
    def get_object(self, pk):
        try:
            return FileModel.objects.get(pk=pk)
        except FileModel.DoesNotExist:
            raise Http404
    def get(self, request, pk):
        file_obj = self.get_object(pk)
        return FileResponse(open(file_obj.file.path, 'rb'))


class SubmissionList(generics.ListAPIView):
    # permission_classes = [IsAuthenticatedOrReadOnly]
    queryset = Submission.objects.all()
    serializer_class = SubmissionSerializer


class SubmissionDetail(generics.RetrieveUpdateAPIView):
    # permission_classes = [IsAuthenticatedOrReadOnly]
    queryset = Submission.objects.all()
    serializer_class = SubmissionSerializer


class Submit(APIView):
    def post(self, request):
        user_id = request.data.get('user')
        contest_id = request.data.get('contest')
        file_id = request.data.get('file')
        user = User.objects.get(id=user_id)
        contest = Contest.objects.get(id=contest_id)
        file = FileModel.objects.get(id=file_id)
        if UserInContest.objects.filter(user=user):
            rel = UserInContest.objects.get(user=user)
            rel.file = file
            rel.save()
        else:
            rel = UserInContest.objects.create(user=user, contest=contest, file=file)
        sub = Submission.objects.create(user=user, contest=contest, file=file)
        userset = UserInContest.objects.filter(contest=contest)
        for it in userset:
            user2 = it.user
            if user == user2:
                continue
            file2 = it.file
            Judge.objects.create(submission=sub, user2=user2, file2=file2)
        serializer = SubmissionSerializer(sub)
        return Response(serializer.data)
            
        
class GetTask(APIView):
    def get(self, request):
        try:
            judge = Judge.objects.filter(status='Waiting').first()
            serializer = JudgeSerializer(judge)
            return Response(serializer.data)
        except Judge.DoesNotExist:
            raise Http404
        

class JudgeList(generics.ListAPIView):
    queryset = Judge.objects.all()
    serializer_class = JudgeSerializer


class JudgeDetail(generics.RetrieveUpdateAPIView):
    # permission_classes = [IsAuthenticatedOrReadOnly]
    queryset = Submission.objects.all()
    serializer_class = JudgeSerializer


class GetJudgeOfSubmission(APIView):
    def get(self, request, pk):
        judgeset = Judge.objects.filter(submission__id=pk)
        serializer = JudgeSerializer(judgeset, many=True)
        return Response(serializer.data)