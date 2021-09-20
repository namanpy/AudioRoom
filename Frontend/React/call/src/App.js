import logo from './logo.svg';
import React from "react"
import './App.css';
import './styles/room.css'
import "./Material/MaterialInput"
import MaterialInputBox from './Material/MaterialInput';
import BoxExpandAnimation from './Material/BoxExpandAnimation';
import AnimatedContainer from './Material/AnimatedContainer';
import ControlPanel from './Modules/ControlPanel';
import UserList from './Modules/UserList';
import VideoContainer from "./Modules/VideoContainer";
class User {
    stream = null;
    peerConnection = null;
    name = null;
    iceCandidateAvailable= false;
    descriptionSet = false;
    iceCandidate = '';
    ref = null;
    constructor(name, stream) {
        this.name = name;
        this.stream = stream;
    }

    addStream(stream) {
        this.stream = stream;
    }

}
class Room {


    static SCREEN_SHARE = 1
    static VIDEO_CALL = 0
    ws = {};

    ///////////////////////////
    //TO IMPLEMENT IN REACT
    onJoinCall = (user)=>{};
    onLocalVideo = (user)=>{};
    onNewUser = (user)=>{};
    onSpeaking = (user)=>{};
    onDisconnect = (user)=>{};
    
    toggleUserVideo(username) {
        this.localStream[username].getVideoTracks().forEach(track => track.enabled = !track.enabled);
    
    }
    toggleUserAudio(username) {
        this.localStream[username].getAudioTracks().forEach(track => track.enabled = !track.enabled);
    }



    //////////////////////////
    maxWidth = 1280;
    maxHeight = 720;
    localStream = {}
    localPeerConnection = {};
    room = null;
    callback = {};

    connectedUsers = []

    handlingUserConnections = false; 


    startSignalingSocket(username) {
     var connectionPromise = new Promise( (resolve, reject) => {
        this.ws[username] = new WebSocket('wss://exrth.com:8080')

            this.callback[username] = {};


            this.ws[username].emit = function(event, data) {

                data.room = this.room;
                var dataToSend = { event : event, data : data };
                this.ws[username].send(JSON.stringify(dataToSend));

            }.bind(this)


            // this.ws.on = (event , fn) => {
                    
            //     this.callback[event] = fn;
                
            // }
            this.ws[username].on = (event, fn) => {
                    
                
                
                this.callback[username][event] = fn; 

                console.log(this.callback)
                
            }

            this.ws[username].onopen = () => {
                resolve();

            }

            this.ws[username].onmessage = async evt => {


                let data = JSON.parse(evt.data)
                // console.log(data);
                // console.log(data.event);


                if(this.callback[username][data.event] != undefined) {
                    this.callback[username][data.event](data);
                }


            }

            this.ws[username].onclose = () => {


            }
        });
        return connectionPromise;
    }


    stopScreenShare(username) {

        this.localPeerConnection[username + "|screen"].close();
        this.ws[username + "|screen"].close();
        delete this.ws[username + "|screen"]
        delete this.callback[username + "|screen"]
    }


    
    async connectToUser(requestingUser, users) {
         for(let i = 0; i < users.length ; i++) { 
            await this.getStreamFromRemotePeers(requestingUser, users[i]);
        }

    }

    userNotConnected(username) {
        for(let i = 0; i < this.connectedUsers.length; i++) {
            if(this.connectedUsers[i].name == username) {

                return false;

            }
        }
        return true;
    }

    // async sendOffer() {

    //     this.ws[username].emit("offer", { offer : this.offer });

    // }
    async joinRoom(username, room, mediaType) {

        this.room = room;
        if(mediaType == Room.VIDEO_CALL) {
            let start = await this.startSignalingSocket(username);

            this.ws[username].emit("auth", { username : username });
            this.onJoinCall(username);

        } else if(mediaType == Room.SCREEN_SHARE) {

            username = username + "|screen";
            let start = await this.startSignalingSocket(username);

            this.ws[username].emit("auth", { username : username});
            this.onJoinCall(username);

        } else {
            return;
        }

        const mediaStreamConstraints = {
              video :  {  width : 1280 , height: 720 } ,   
              audio : true
        };



        if(mediaType == Room.VIDEO_CALL) {
            this.localStream[username] =  await navigator.mediaDevices.getUserMedia(mediaStreamConstraints);
            //Disabled by default
            this.toggleUserAudio(username);
            this.toggleUserVideo(username);
        } else {
            this.localStream[username] =  await navigator.mediaDevices.getDisplayMedia(mediaStreamConstraints);

        }

  
        
        this.onLocalVideo(username);


        const configuration = {'iceServers': [{ 'urls': 'stun:stun.l.google.com:19302' }]}

        //Create Peer Connection
        this.localPeerConnection[username] = new RTCPeerConnection(configuration);


        this.localStream[username].getTracks().forEach(track => {
            this.localPeerConnection[username].addTrack(track, this.localStream[username]);
        });


        this.offer = await this.localPeerConnection[username].createOffer();
        await this.localPeerConnection[username].setLocalDescription(this.offer);

        this.localPeerConnection[username].onicecandidate = (event) => {
            if (event.candidate) {

                this.ws[username].emit("iceCandidate", event.candidate.toJSON())

            } else {
            }
        }

        this.localPeerConnection[username].oniceconnectionstatechange = event => {
            if (this.localPeerConnection[username].iceConnectionState === 'connected') {
                if(this.userNotConnected(username)) {
                    this.ws[username].emit("requestUserList", { room : this.room });
                    
                }
            }
        }


        this.ws[username].on("answer",async (data) => {

            try {
                console.log("DEBUG", "Got an answer")
                let sdp =  await  this.localPeerConnection[username].setRemoteDescription({ type: "answer", sdp : data.answer});
            } catch(err) {
                console.log("Could'nt set remote description ", err)
            }
        });

        this.ws[username].on("iceCandidate",  async (data) => {

            try {

                console.log("OnICE");
                await this.localPeerConnection[username].addIceCandidate(new RTCIceCandidate({
                    candidate: data.iceCandidate,
                    sdpMid: data.mid,
                    sdpMLineIndex: data.index, 
                }));

            } catch(err) {
                console.log("Couldn't add iceCandidate ", err)
            }
        });

        if(!this.handlingUserConnections) {
            this.ws[username].on("userList",   async (data)=>{
                let usersToConnect = [];
                for(let i = 0; i < data.users.length; i++) {
                    if(this.ws[data.users[i]]) {

                    } else {
                        usersToConnect.push(data.users[i]);
                    }

                }
                this.connectToUser(username, usersToConnect);

            });


            this.ws[username].on("newUser", async (data) => {
                if(!this.ws[data.user]) {
                    this.connectToUser(username, [data.user]);
                }
            });

            this.ws[username].on("speaking", async (data)=> {

                this.onSpeaking(data.speakers);

            });

            
            this.handlingUserConnections = true;
        }


        this.ws[username].emit("joinRoom", { room : room })
        this.ws[username].emit("offer", { offer : this.offer });


    }
    removeUser(requestingUser, username) {
        
        delete this.callback[requestingUser][username + "~requestUserMedia"]
        delete this.callback[requestingUser][username + "~offer"]
        delete this.callback[requestingUser][username + "~iceCandidate"]
        delete this.callback[requestingUser][username + "~disconnect"]
        var userIdx = -1
        for(let i = 0; i < this.connectedUsers.length; i++) {
            if(this.connectedUsers[i].name == username) {
                userIdx = i;
                break;
            }
        }
        if(userIdx >= 0) {
            this.connectedUsers.splice(userIdx, 1);
        }

    }


    async getStreamFromRemotePeers( requestingUser, username) {

        
        var user = new User(username, null);

        let  requestUserMedia = ( user) => {

            this.ws[requestingUser].emit("~requestUserMedia",  { user : user.name });
        }


        this.ws[requestingUser].on(user.name + "~offer" , async (data) => {
            try {
                //console.log("[GOT OFFER !!!!]");
                const configuration = {'iceServers': [{ 'urls': 'stun:stun.l.google.com:19302' }]}

               // Create Peer Connection
                var peerConnection_ = new RTCPeerConnection(configuration);

                user.peerConnection = peerConnection_;

                user.peerConnection = peerConnection_;
                user.stream = new MediaStream();
                user.iceCandidateAvailable= false;
                user.descriptionSet = false;
                user.iceCandidate = null;

                user.peerConnection.onicecandidate = (event) => {
                    if (event.candidate) {
                        console.log("~onicecandidate called");
                        let candidate = event.candidate.toJSON();
                        candidate["user"] = data.user;
                        this.ws[requestingUser].emit("~iceCandidate", candidate);
                    } else {
                    }
                }
                user.peerConnection.oniceconnectionstatechange = event => {
                   // console.log("~Connection state : ", this.remotepeers.current[data.user].peerConnection.iceConnectionState);
                    if (user.peerConnection.iceConnectionState === 'connected') {
                        if(this.userNotConnected(user.name)) {

                            this.connectedUsers =  [ ...this.connectedUsers,  user];
                            this.onNewUser(user);

                        }

                    }
                }

                user.peerConnection.ontrack = (event) => {

                   //  console.log("Streams ",event.streams, "###################################################3 <???????????????????????????");


                    user.stream.addTrack(event.track, this.remoteStream);
                    // var contains = false;
                    // for(let i = 0; i < this.state.peers.length; i++) {
                    //     if(this.state.peers[i].user == data.user) {
                    //         contains = true;
                    //         break;
                    //     }
                    // }
                    // if(!contains) {
                    //     this.setState( { peers : [...this.state.peers, { user : data.user, videoRef : React.createRef()  }] });
                    // }
                }

                //console.log("[~] sdp : ", data.offer)
                let sdp =  await  user.peerConnection.setRemoteDescription({ type: "offer", sdp : data.offer});
                //console.log("~Remote description set.")

                let answer = await  user.peerConnection.createAnswer()
                await  user.peerConnection.setLocalDescription(answer);
                //console.log("[~] sdp : ", answer.sdp);

                user.descriptionSet = true;

                if(user.iceCandidateAvailable == true) {
                    user.iceCandidateAvailable = false;
                    try {
                        await user.peerConnection.addIceCandidate(user.iceCandidate);
                        console.log("~Added iceCandidate ");
                    } catch(err) {
                        console.log("~Couldn't add iceCandidate ", err)
                    }
                }


               //console.log("~Local description set.")
                this.ws[requestingUser].emit("~answer", { answer : answer , user : data.user } );

            } catch(err) {
                console.log("~Couldn't set up peer connection ", err)
            }

        });

        this.ws[requestingUser].on(user.name  + "~iceCandidate", async (data) => {
            if(!user.descriptionSet) {
                user.iceCandidateAvailable= true;
                user.iceCandidate  = new RTCIceCandidate({
                    candidate: data.iceCandidate,
                    sdpMid: data.mid,
                    sdpMLineIndex: data.index,
                });

                return;

            }
            try {
                await user.peerConnection.addIceCandidate(new RTCIceCandidate({
                    candidate: data.iceCandidate,
                    sdpMid: data.mid,
                    sdpMLineIndex: data.index,
                }));
                //console.log("~Added iceCandidate ");
            } catch(err) {
                console.log("~Couldn't add iceCandidate ", err)
            }
        });

        this.ws[requestingUser].on(user.name + "~disconnect", (data)=>{

            this.removeUser(requestingUser, user.name);

            this.onDisconnect(user);

        });



        requestUserMedia(user);

    }


}


class App extends React.Component {

  constructor(props) {
    super()

    this.state = {
        users : [],
        localStreams : [],
        speakers : [],
        username  : "",
        room : "",
        inCall : false,
        closeInputContainer : false


    };
    this.room = new Room();
    
    this.room.onJoinCall = (username)=>{
        this.setState({ inCall : true });
    };

    this.room.onLocalVideo = (username)=>{

        this.setState({ localStreams : [ ...this.state.localStreams, this.room.localStream[username]] });

    };
 

    this.room.onNewUser = (user)=>{

        this.setState( {  users : [ ...this.room.connectedUsers] } )
        

    };

    this.room.onDisconnect = (user) => {
        this.setState( { users : [...this.room.connectedUsers] } );

    };

    this.room.onSpeaking = (userlist)=>{
       // console.log(userlist);
        for(let i = 0 ; i < this.state.users.length; i++) {

            if(userlist.includes(this.state.users[i].name)) {
                this.state.users[i].ref.classList.add("speaking-border");
            } else {

                this.state.users[i].ref.classList.remove("speaking-border");


            }

        }
    };




    //this.joinRoom = this.joinRoom.bind(this);
    //this.remotepeers = this.remotepeers.bind(this);

  }

    setInCall() {
        this.setState({ inCall : true });
    }






    componentDidMount() {

        //this.room.startSignalingSocket();
    }


    // setSrcObject(ref, usr) {

    //     if(ref == undefined ) {
    //         console.log("Undefined ref");
    //         return;
    //     }
	// console.log(this.remotepeers.current , data.user);
    //     ref.srcObject = this.remotepeers.current[data.user].stream;

    // }
    calculateVideoWidth() {

    }
    calculateVideoHeight() {

    }

  render() {

    if(this.state.inCall) {
        return (

            <div>
                <BoxExpandAnimation>
                    <UserList users={this.state.users} srcUser={this.state.username} >


                    </UserList>

                    <VideoContainer users={this.state.users} room={this.state.room}></VideoContainer>
                    
                    <ControlPanel onScreenShare={ ()=> this.room.joinRoom(this.state.username, this.state.room, Room.SCREEN_SHARE) }   onScreenShareStop={ () => this.room.stopScreenShare(this.state.username) } onToggleVideo={ () => this.room.toggleUserVideo(this.state.username)}  onToggleAudio={()=>this.room.toggleUserAudio(this.state.username)}>
                        {this.state.localStreams.map( (stream, idx)=> {

                            //console.log("displaying local streams" , stream);
                            return <video key={idx} class="videosrc" ref={ ref => {  if(ref) { ref.srcObject = stream; } }} autoPlay muted playsInline autoplay="autoplay" ></video>
 


                        })}
                    
                    </ControlPanel>
                    

                </BoxExpandAnimation>
            </div>
    
        );

        
    } else {

        return (
        <AnimatedContainer close={this.state.closeInputContainer} onfinish={ () => this.room.joinRoom(this.state.username, this.state.room, Room.VIDEO_CALL)}>
            <div class="input-section" >
                <div>
                        <div >
                            <MaterialInputBox label="Username" name="username"  onChange={ (e) => { this.setState( { username  : e.target.value } ) } } />
                            <br/>
                            <MaterialInputBox label="Room" name="room"  onChange={ (e) => { this.setState( { room  : e.target.value } ) } } />
                        </div>
                        <br/>

                        <div >
                            {/* <label for="share">Share?</label> */}
                            {/* <input type="checkbox" name="share" value={this.state.share} onChange={ (e) => {this.setState( { share : !this.state.share } ) } } /> */}

                            <button class="material-button" onClick={ ()=>  { this.setState( { closeInputContainer : true}); } } > Join </button>
                        </div>
                    </div>
            </div>

        </AnimatedContainer>


        );

    }



  }
}
export default App;


