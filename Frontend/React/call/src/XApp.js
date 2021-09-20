import logo from './logo.svg';
import React from "react"
import './App.css';

class App extends React.Component {






  constructor(props) {
    super()
    this.peerConnection = null;
    this.localStream = new MediaStream();
    this.localVideo= React.createRef();
//     this.remoteStream = new MediaStream();
//     this.remoteVideo = React.createRef();
    this.remotepeers = React.createRef({});
    this.remotepeers.current = {};

    this.callback = {};

    this.state = {

        peers : []

    };

    this.setoffer = `

    `

    this.test = `v=0
o=- 1746110361190241258 2 IN IP4 127.0.0.1
s=-
t=0 0
a=extmap-allow-mixed
a=msid-semantic: WMS
m=audio 9 UDP/TLS/RTP/SAVPF 111 110
c=IN IP4 0.0.0.0
a=rtcp:9 IN IP4 0.0.0.0
a=ice-lite
a=ice-ufrag:eWm3
a=ice-pwd:KvSixMwqRu4aZDUV1h3igG
a=fingerprint:sha-256 EC:C4:12:A8:2F:07:93:D1:AA:34:E2:5E:1E:60:14:DB:8B:FB:9B:B3:65:FC:D6:76:8D:CC:85:DA:9A:D3:7E:5E
a=setup:passive
a=mid:0
a=extmap:1 urn:ietf:params:rtp-hdrext:ssrc-audio-level
a=extmap:2 urn:ietf:params:rtp-hdrext:csrc-audio-level
a=extmap:3 urn:ietf:params:rtp-hdrext:sdes:mid
a=recvonly
a=rtcp-mux
a=rtpmap:111 opus/48000/2
a=rtcp-fb:111 transport-cc
a=rtpmap:110 telephone-event/48000
`
//a=extmap:2 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time
//a=extmap:2 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01
//a=extmap:4 urn:ietf:params:rtp-hdrext:sdes:rtp-stream-id
//a=extmap:5 urn:ietf:params:rtp-hdrext:sdes:repaired-rtp-stream-id
    this.joinRoom = this.joinRoom.bind(this);
//     this.remotepeers = this.remotepeers.bind(this);

  }
    startSignalingSocket(room) {

        this.ws = new WebSocket('ws://192.168.1.47:9690')

        this.ws.emit = function(event, data) {

            var dataToSend = { event : event, data : data };
            this.ws.send(JSON.stringify(dataToSend));

        }.bind(this)


        this.ws.on = (event , fn) => {

            this.callback[event] = fn;
            console.log("fn", this.callback[event]);
        }

        this.ws.onopen = () => {


        }

        this.ws.onmessage = async evt => {



            let data = JSON.parse(evt.data)

            console.log(data.event);
            this.callback[data.event](data);


        }

        this.ws.onclose = () => {


        }
    }

    async getVideoFromRemotePeers(user) {


        var user = user;

        let  requestUserMedia = ( user) => {

            this.ws.emit("~requestUserMedia",  { user : user });
        }


        this.ws.on("~offer", async (data) => {
//             try {

                const configuration = {'iceServers': [{ 'urls': 'stun:stun.l.google.com:19302' }]}

                //Create Peer Connection
                var peerConnection_ = new RTCPeerConnection(configuration);

                this.remotepeers.current[data.user].peerConnection = peerConnection_;
                this.remotepeers.current[data.user].stream = new MediaStream();


                 this.remotepeers.current[data.user].peerConnection.onicecandidate = (event) => {
                    if (event.candidate) {
                        console.log("~onicecandidate called");
                        let candidate = event.candidate.toJSON();
                        candidate["user"] = data.user;
                        this.ws.emit("~iceCandidate", candidate);
                    } else {
                    }
                }
                 this.remotepeers.current[data.user].peerConnection.oniceconnectionstatechange = event => {
                    console.log("~Connection state : ", this.remotepeers.current[data.user].peerConnection.iceConnectionState);
                    if (this.remotepeers.current[data.user].peerConnection.iceConnectionState === 'connected') {

                    }
                }

                 this.remotepeers.current[data.user].peerConnection.ontrack = (event) => {

                    this.remotepeers.current[data.user].stream.addTrack(event.track, this.remoteStream);
                    this.setState( { peers : [...this.state.peers, { user : data.user, videoRef : React.createRef()  }] });
                }

                console.log("[~] sdp : ", data.offer)
                let sdp =  await  this.remotepeers.current[user].peerConnection.setRemoteDescription({ type: "offer", sdp : data.offer});
                console.log("~Remote description set.")

                let answer = await  this.remotepeers.current[data.user].peerConnection.createAnswer()
                await  this.remotepeers.current[data.user].peerConnection.setLocalDescription(answer);
//                console.log("~Local description set.")
                this.ws.emit("~answer", { answer : answer , user : data.user } );

//             } catch(err) {
//                 console.log("~Couldn't set up peer connection ", err)
//             }

        });

        this.ws.on("~iceCandidate", async (data) => {
            try {
                await this.remotepeers.current[data.user].peerConnection.addIceCandidate(new RTCIceCandidate({
                    candidate: data.iceCandidate,
                    sdpMid: data.mid, // don't make it up, you get this in onicecandidate
                    sdpMLineIndex: data.index, // don't make it up, you get this in onicecandidate
                }));
                console.log("~Added iceCandidate ");
            } catch(err) {
                console.log("~Couldn't add iceCandidate ", err)
            }
        });

        requestUserMedia(user);

    }



    async sendOffer() {

        this.ws.emit("offer", { offer : this.offer });

    }

    componentDidMount() {


    }

    async joinRoom(room) {
        this.startSignalingSocket(room);

        const mediaStreamConstraints = {
        //video : true,
              audio: {
    autoGainControl: false,
    channelCount: 1,
    echoCancellation: false,
    latency: 0,
    noiseSuppression: false,
    sampleRate: 48000,
    sampleSize: 16,
    volume: 1.0
  }

        };

        this.localStream =  await navigator.mediaDevices.getUserMedia(mediaStreamConstraints);
        this.localVideo.current.srcObject = this.localStream;


        const configuration = {'iceServers': [{ 'urls': 'stun:stun.l.google.com:19302' }]}

        //Create Peer Connection
        this.peerConnection = new RTCPeerConnection(configuration);


        //Add User Media
        this.localStream.getTracks().forEach(track => {
            this.peerConnection.addTrack(track, this.localStream);
        });


        this.offer = await this.peerConnection.createOffer();
        await this.peerConnection.setLocalDescription(this.offer);
        this.peerConnection.setRemoteDescription({ type : "answer", sdp : this.test });
        console.log("OFFER \n");
        console.log(this.offer.sdp);


        this.peerConnection.onicecandidate = (event) => {
//             if (event.candidate) {
//
//                 this.ws.emit("iceCandidate", event.candidate.toJSON())
//             } else {
//             }
        }
        this.peerConnection.oniceconnectionstatechange = event => {
//             if (this.peerConnection.iceConnectionState === 'connected') {
//                 this.ws.emit("requestUserList", { room : room });
//             }
            console.log(this.peerConnection.iceConnectionState);
        }


        this.ws.on("answer", async (data) => {

            try {
                let sdp =  await  this.peerConnection.setRemoteDescription({ type: "answer", sdp : data.answer});
                console.log("Remote description set.\n" , data.answer)
            } catch(err) {
                console.log("Could'nt set remote description ", err)
            }
        });

        this.ws.on("iceCandidate", async (data) => {

            try {

                console.log(data);
                await this.peerConnection.addIceCandidate(new RTCIceCandidate({
                    candidate: data.iceCandidate,
                    sdpMid: data.mid, // don't make it up, you get this in onicecandidate
                    sdpMLineIndex: data.index, // don't make it up, you get this in onicecandidate
                }));
                console.log("Added iceCandidate ");

            } catch(err) {
                console.log("Couldn't add iceCandidate ", err)
            }
        });


        this.ws.on("userList",  async (data)=>{

            for(let i = 0; i < data.users.length ; i++) {

                this.remotepeers.current[data.users[i]] = {};
                await this.getVideoFromRemotePeers(data.users[i]);

            }
            console.log(this.remotepeers)
        });

        this.ws.on("newUser", async (data) => {
            this.remotepeers.current[data.user] = {};
            await this.getVideoFromRemotePeers(data.user);
        });

        this.peerConnection.ontrack = (e) => {
            console.log("HOW???????????????????????????????????????")
        }

//         this.ws.emit("joinRoom", { room : room })

    console.log("adding ice cand");


//         this.sendOffer();
//     .then((stream) => { this.gotLocalMediaStream(stream)}).catch((err)=> {this.handleLocalMediaStreamError(err)});
//
//         }

    }
    async  addIceCandidate() {
                await this.peerConnection.addIceCandidate(new RTCIceCandidate({
                    candidate: "candidate:6695075 1 udp 2130706431 192.168.1.38 9690 typ host",
                    sdpMid: 0, // don't make it up, you get this in onicecandidate
                    sdpMLineIndex: 0, // don't make it up, you get this in onicecandidate
                }));
    }

    setSrcObject(ref, data) {

        ref.srcObject = this.remotepeers.current[data.user].stream;

    }
  render() {
    return (

        <div>
            <h1>Realtime communication with WebRTC</h1>

            <button onClick={ ()=> this.joinRoom("room1") } > Join </button>
            <button onClick={ ()=> this.addIceCandidate() } >add ice candidate </button>

            <video ref={ this.localVideo } autoPlay muted playsInline controls={true} autoplay="autoplay" width="640" height="480"></video>


            <div>
                {this.state.peers.map((d, idx) => {

                    var a = <audio key={idx} ref={ ref => { this.setSrcObject(ref, d) }} autoPlay playsInline controls={true} autoplay="autoplay" ></audio>;



                    return a;
                })}
            </div>




            <script src="js/main.js"></script>
        </div>


    );
  }
}
export default App;
