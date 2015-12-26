__author__ = 'ctewani'

import tweepy
from collections import defaultdict
import csv
import plotly.plotly as py
from plotly.graph_objs import *

nomineeSeedDict = defaultdict(list)
emoSeedDict = defaultdict(list)
nomineeEmoTweetDict = defaultdict(lambda: defaultdict(list))
emoList = ['positive','negative']
emoSeedTweetDict = defaultdict(list)

def getTweets():
   

    consumer_key = 'consumer_key'
    consumer_secret = 'consumer_secret'
    access_token = 'access_token'
    access_token_secret = 'access_token_secret'

    auth = tweepy.OAuthHandler(consumer_key, consumer_secret)
    auth.set_access_token(access_token, access_token_secret)
    tweet_output = open('./tweet_output.txt','a')
    tweets = []
    api = tweepy.API(auth)

    #================================ Tweepy Cursor ================================
    #query = 'deserved emmys 2015 OR deserved emmy 2015 OR win emmy 2015 OR win emmys 2015 OR won emmy 2015 OR won emmys 2015 OR lost emmy 2015 OR lost emmy 2015'
    query = 'happy emmy 2015 OR happy emmys 2015 OR disappointed emmy 2015 OR should have won emmys 2015 OR congratulations emmy 2015 OR congrats emmy 2015 OR deserve emmy 2015 OR love emmy 2015'
    #query = 'excited emmys'
    max_tweets = 200
    for subQuery in query.split(' OR '):
        searched_tweets = [status for status in tweepy.Cursor(api.search, q=subQuery, wait_on_rate_limit = True, include_rts = False, lang="en").items(max_tweets)]
        count = 0
        for tweet in searched_tweets:
            if not tweet.text.startswith('RT'):
                #print tweet
                tweet_output.write(tweet.text.encode('ascii', 'ignore')+'\n')
                count += 1
        #print searched_tweets
        print "Number of tweets crawled for ",subQuery, " : ",count
    tweet_output.close()


    '''
    ================================ Tweepy Streaming ================================

    #This line filter Twitter Streams to capture data by the keywords: 'python', 'javascript', 'ruby'
    #stream.filter(track=['emmys', 'emmys2015'])

    #override tweepy.StreamListener to add logic to on_status
    class MyStreamListener(tweepy.StreamListener):
        #def on_data(self, data):
        #    print data
        #    return True

        def on_status(self, status):
            print(status.text)

    myStreamListener = MyStreamListener()
    myStream = tweepy.Stream(auth = api.auth, listener=myStreamListener, timeout=None)
    myStream.filter(track=['Emmys'])

    '''

def parseNomineeList():
    nomineeList = open('./Data/EmmyN.txt','r').readlines()
    nomineeKey = ''
    newNominee = True
    for nomineeListLine in nomineeList:
        if nomineeListLine == '\n':
            newNominee = True
            continue
        nomineeListLine = nomineeListLine.lower()
        if newNominee:
            nomineeKey = nomineeListLine.replace("\n",'').rstrip()
            newNominee = False
        nomineeSeedDict[nomineeKey].append(nomineeListLine.replace("\n",'').rstrip())

    '''
    for keys in nomineeSeedDict.keys():
        print keys,"==>",nomineeSeedDict[keys]
        print
    '''

def parseEmoList():
    positiveList = open('./Data/pos.txt','r').read().split('\n');
    negativeList = open('./Data/neg.txt','r').read().split('\n');
    for positive in positiveList:
        emoSeedDict['positive'].append(positive.lower())
    for negative in negativeList:
        emoSeedDict['negative'].append(negative.lower())

    '''
    for e in emoSeedDict.keys():
        print e,"==>",emoSeedDict[e]
        print
    '''

def cleanTweets():
    tweets = open('./Data/tweet_output.txt','r').read().split('\n')
    tweets_clean = open('./Data/tweet_output_clean.txt','a')
    for tweet in tweets:
        cleanTweet = ' '.join([word.lower() for word in tweet.split() if not word.startswith('http')])
        tweets_clean.write(cleanTweet+'\n')
    tweets_clean.close()

def multiEmoMultiNominee(tweet, emoFoundDict, nomineeFoundDict):
    nomineeFoundList = []
    for keys in emoFoundDict.keys():
        index = tweet.index(keys)
        indexNom = []
        for nomineeSeedKey in nomineeFoundDict.keys():
            nomineeFoundList.append(nomineeFoundDict[nomineeSeedKey])
            indexNom.append(tweet.index(nomineeSeedKey))



def parseTweets():
    # This is where magic happens!
    tweets = open('./Data/tweet_output_clean.txt','r').read().split('\n')
    multiNomineeTweet = open('./Output/multiNominee.txt','a')
    multiEmoMultiNomineeTweet = open('./Output/multiEmoMultiNominee.txt','a')
    multiEmoTweet = open('./Output/multiEmoTweet.txt','a')
    missingEmoTweet = open('./Output/missingEmoTweet.txt','a')
    missingNominee = open('./Output/missingNominee.txt','a')

    emoNomineeMatchCount = 0
    multiEmoTweetCount = 0
    multiNomineeTweetCount = 0
    multiNomineeMultiEmoCount = 0
    missingEmoCount = 0
    missingNomineeCount = 0
    for tweet in tweets:
        emoCount = 0
        nomineeCount = 0
        nomineeFound = ''
        emoSeedFound = ''
        emoFound = ''
        isEmoFound = False
        isNomineeFound = False
        emoFoundDict = defaultdict(str)
        nomineeFoundDict = defaultdict(str)
        for emo in emoList:
            for emoSeed in emoSeedDict[emo]:
                if emoSeed in tweet:
                    emoFoundDict[emoSeed] = emo
                    emoCount += 1
                    if not isEmoFound:
                        emoSeedFound = emoSeed
                        emoFound = emo
                        isEmoFound = True
                    break

        if isEmoFound:
            for nominee in nomineeSeedDict.keys():
                for nomineeSeed in nomineeSeedDict[nominee]:
                    if nomineeSeed in tweet:
                        nomineeFoundDict[nomineeSeed] = nominee
                        nomineeCount += 1
                        if not isNomineeFound:
                            nomineeFound = nominee
                            isNomineeFound = True
                        break

        if isEmoFound and isNomineeFound:
            #emoNomineeMatchCount += 1
            #nomineeEmoTweetDict[nomineeFound][emoFound].append(tweet)
            if emoCount > 1 and nomineeCount > 1:
                #multiEmoMultiNominee(tweet, emoFoundDict, nomineeFoundDict)
                multiNomineeMultiEmoCount += 1
                multiEmoMultiNomineeTweet.write(tweet+'\n')
            elif emoCount > 1 and nomineeCount == 1:
                multiEmoTweetCount += 1
                multiEmoTweet.write(tweet+'\n')
            elif nomineeCount > 1 and emoCount == 1:
                for nomineeKey in nomineeFoundDict.keys():
                    emoNomineeMatchCount += 1
                    nomineeEmoTweetDict[nomineeFoundDict[nomineeKey]][emoFound].append(tweet)
                multiNomineeTweetCount += 1
                multiNomineeTweet.write(tweet+'\n')
            else:
                emoNomineeMatchCount += 1
                nomineeEmoTweetDict[nomineeFound][emoFound].append(tweet)
        elif isEmoFound:
            missingNomineeCount += 1
            missingNominee.write(tweet+'\n')
        else:
            missingEmoCount += 1
            missingEmoTweet.write(tweet+'\n')
    multiNomineeTweet.close()
    multiEmoMultiNomineeTweet.close()
    multiEmoTweet.close()
    missingEmoTweet.close()
    missingNominee.close()



    print "multiEmoTweetCount",multiEmoTweetCount
    print "multiNomineeTweetCount", multiNomineeTweetCount
    print "multiNomineeMultiEmoCount", multiNomineeMultiEmoCount
    print "missingEmoCount", missingEmoCount
    print "missingNomineeCount",missingNomineeCount

    print
    print "emoNomineeMatchCount",emoNomineeMatchCount
    print
    print

def writeAndPlot():
    csvfile = open('./Output/nomineeEmoTweet.csv','a')
    fieldnames = ['Nominee','Emotion Type','Tweet']
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
    writer.writeheader()
    emoTweetsLength = defaultdict(list)
    for nomineeKey in nomineeEmoTweetDict.keys():
        for emoKey in emoList:
            print nomineeKey, emoKey, "==>",len(nomineeEmoTweetDict[nomineeKey][emoKey])
            emoTweetsLength[emoKey].append(len(nomineeEmoTweetDict[nomineeKey][emoKey]))
            for tweet in nomineeEmoTweetDict[nomineeKey][emoKey]:
                writer.writerow({'Nominee':nomineeKey,'Emotion Type':emoKey,'Tweet':tweet})
    traceList = []
    for emoKey in emoList:
        for index in range(0,len(emoTweetsLength[emoKey])):
            if emoTweetsLength[emoKey][index] > 150:
                emoTweetsLength[emoKey][index] = 72
        trace = Bar(
            x= nomineeEmoTweetDict.keys(),
            y=emoTweetsLength[emoKey],
            name=emoKey
        )
        traceList.append(trace)
    data = Data(traceList)
    layout = Layout(
            barmode='group',
        title='How Twitter reacts to Emmys', xaxis=XAxis(
        title='Nominees'), yaxis = YAxis(title='Tweet Count') )

    fig = Figure(data=data, layout=layout)
    plot_url = py.plot(fig, filename='twitter-reaction-emmys')

parseNomineeList()
parseEmoList()
#cleanTweets()
parseTweets()
writeAndPlot()



