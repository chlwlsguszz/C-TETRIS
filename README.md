# C-TETRIS


<div id="2"></div>

##  개요
- 2학년 개인 Term-Project -> C 언어로 제작한 TETRIS 게임 
- 2021.06 제작, 1개월 소요
- Visual Studio 에서 작업
- 주요 목표 : 옛날 고전게임처럼 아기자기 하고 재미있게 만들기 
- 게임 플레이 동영상 (https://www.youtube.com/watch?v=xKieMqaREf0)

##  주요 기능

키보드 입력
1. 방향키 ←→ 좌우 이동, ↑ 회전, ↓ 한 칸 밑으로
2. 스페이스바 : 즉시 가장 밑으로
3. P: 일시정지 R: 재시작 ESC: 종료

게임 로직
1. 천장에 닿으면 종료
2. 한 줄이 꽉 차면 파괴 
3. 점수, 레벨 시스템 (블럭이 내려오는 속도 변경)
4. 블록 바닥 충돌 -> 고정 전 약간의 딜레이 (실제 게임처럼 움직일 시간을 줌)

비주얼 작업
1. 한 줄이 꽉차면 파괴 -> 이펙트 생성
2. 블록 색상을 다양하게 설정
3. 다음 블록을 미리 보여줌
4. 떨어질 예상 장소를 보여줌
5. 배경음악, 게임 시작/종료 효과음
6. 게임 종료시 이름, 점수 파일 저장 -> 순위 정렬해서 화면에 보여줌
   
<br />

##  게임 화면

### 시작화면 
![Image](https://github.com/user-attachments/assets/b43c186d-3a8b-4f55-9825-f70341a21a13)

### 게임 플레이
![Image](https://github.com/user-attachments/assets/a2ef034c-3beb-4116-b84c-a01d81b02c22)

### 게임 종료
![Image](https://github.com/user-attachments/assets/fbec08d3-1554-45c5-b616-238afb3aa044)
